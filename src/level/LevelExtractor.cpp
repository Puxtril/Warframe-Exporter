#include "level/LevelExtractor.h"

using namespace WarframeExporter::Level;

LevelExtractor*
LevelExtractor::getInstance()
{
	static LevelExtractor* instance = new LevelExtractor();
	return instance;
}

LevelExternal
LevelExtractor::getLevelExternal(LotusLib::FileEntry& fileEntry)
{
	LevelReader* levelReader = g_enumMapLevel[fileEntry.commonHeader.type];
	
	LevelExternal external;
	external.landscapeIndex = -1;
	levelReader->readHeader(fileEntry.headerData, external.header);
	levelReader->readBody(fileEntry.bData, external.header, external.body);

	findLandscape(external);
	return external;
}

LevelInternal
LevelExtractor::convertToInternal(LotusLib::FileEntry& fileEntry, LevelExternal& levelExternal)
{
	LevelInternal bodyInt;
	LevelConverter::convertToInternal(levelExternal.header, levelExternal.body, fileEntry.internalPath, bodyInt);
	LevelConverter::convertLandscapeToInternal(levelExternal, bodyInt);
	return bodyInt;
}

void
LevelExtractor::findExtraAttributes(LotusLib::PackagesReader& pkgs, LevelExternal& levelExternal)
{
	LotusLib::PackageReader pkg = pkgs.getPackage("Misc").value();

	for (size_t i = 0; i < levelExternal.header.levelObjs.size(); i++)
	{
		std::vector<char>& curAttributes = levelExternal.body.attributes[i];

		LotusLib::FileEntry entry = pkg.getFile(levelExternal.header.levelObjs[i].objTypePath, LotusLib::READ_EXTRA_ATTRIBUTES);
		if (entry.extra.attributes.size() > curAttributes.size())
		{
			size_t oldSize = curAttributes.size();
			curAttributes.resize(oldSize + entry.extra.attributes.length());
			memcpy(&curAttributes[oldSize-1], entry.extra.attributes.c_str(), entry.extra.attributes.length());
		}
	}
}

void
LevelExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options)
{
	LevelExternal levelExt = getLevelExternal(fileEntry);
	pkgs.initilizePackagesBin();
	findExtraAttributes(pkgs, levelExt);
	LevelInternal levelInt = convertToInternal(fileEntry, levelExt);

	m_logger.info("Level object count: " + std::to_string(levelInt.objs.size()));

	Document gltfOut = createGltfCombined(pkgs, levelInt, options);

	if (gltfOut.buffers.size() > 1)
	{
		for (size_t i = 0; i < gltfOut.buffers.size(); i++)
		{
			Buffer& currentBuf = gltfOut.buffers[i];
			currentBuf.uri = outputPath.stem().string() + std::to_string(i) + ".bin";
		}
	}

	if (!options.dryRun)
		fx::gltf::Save(gltfOut, outputPath, gltfOut.buffers.size() > 1 ? false : true);
}

Document
LevelExtractor::createGltfCombined(LotusLib::PackagesReader& pkgs, LevelInternal& bodyInt, ExtractOptions options)
{
	Document outGltf;

	LotusLib::PackageReader miscPkg = pkgs.getPackage("Misc").value();

	if (!bodyInt.landscape.landscapePath.empty())
		addLandscapeToGltf(outGltf, bodyInt, pkgs);

	for (size_t x = 0; x < bodyInt.objs.size(); x++)
	{
		// >3GB
		if (outGltf.buffers.size() > 0 && outGltf.buffers.back().data.size() > 3221225472)
			outGltf.buffers.resize(outGltf.buffers.size() + 1);

		LevelObjectInternal& curLevelObj = bodyInt.objs[x];

		if (curLevelObj.meshPath == "")
			continue;

		bool isHlod = curLevelObj.objTypePath.length() >= 19 && curLevelObj.objTypePath.compare(curLevelObj.objTypePath.length() - 19, 19, "HLODAggregateEntity") == 0;
		if (
			(isHlod && options.levelHlodExtractMode == LevelHlodExtractMode::IGNORE_HLOD) ||
			(!isHlod && options.levelHlodExtractMode == LevelHlodExtractMode::ONLY_HLOD)
		) { continue; }

		try
		{
			LotusLib::FileEntry curLevelObjFile = miscPkg.getFile(curLevelObj.meshPath);

			if (curLevelObjFile.headerData.getLength() == 0)
			{
				m_logger.warn(spdlog::fmt_lib::format("Object doesn't exist: {}", curLevelObj.meshPath));
				continue;
			}

			if (WarframeExporter::Model::g_enumMapModel.at(pkgs.getGame(), curLevelObjFile.commonHeader.type) == nullptr)
			{
				m_logger.warn(spdlog::fmt_lib::format("Skipping unsupported type {}: {}", curLevelObjFile.commonHeader.type, curLevelObj.meshPath));
				continue;
			}

			WarframeExporter::Model::ModelHeaderExternal headerExt;
			WarframeExporter::Model::ModelBodyExternal bodyExt;
			WarframeExporter::Model::ModelExtractor::getInstance()->extractExternal(curLevelObjFile, pkgs.getGame(), headerExt, bodyExt);

			if (headerExt.meshInfos.size() == 0)
				continue;

			// Gltf exporter doesn't like multiple rigged models
			headerExt.boneTree.clear();

			WarframeExporter::Model::ModelHeaderInternal headerInt;
			WarframeExporter::Model::ModelBodyInternal bodyInt;
			auto vertexColors = WarframeExporter::Model::ModelExtractor::getInstance()->getVertexColors(curLevelObj.meshPath, miscPkg, options.extractVertexColors);
			WarframeExporter::Model::ModelConverter::convertToInternal(headerExt, bodyExt, curLevelObjFile.commonHeader.attributes, vertexColors, headerInt, bodyInt, WarframeExporter::Model::g_enumMapModel.at(pkgs.getGame(), curLevelObjFile.commonHeader.type)->ensmalleningScale(), curLevelObj.meshPath);

			LevelConverter::replaceOverrideMaterials(curLevelObj.materials, headerInt);

			LevelExporterGltf::addModel(outGltf, headerInt, bodyInt, bodyExt, curLevelObj);
		}
		catch (std::exception& ex)
		{
			if (curLevelObj.meshPath.length() > 5)
				m_logger.error(spdlog::fmt_lib::format("{}: {}", ex.what(), curLevelObj.meshPath));
			continue;
		}
	}

	return outGltf;
}

void
LevelExtractor::findLandscape(LevelExternal& levelExternal)
{
	for (size_t i = 0; i < levelExternal.header.levelObjs.size(); i++)
	{
		if (levelExternal.header.levelObjs[i].objTypePath == "/EE/Types/Effects/Landscape")
		{
			levelExternal.landscapeIndex = i;
			break;
		}
	}
}

void
LevelExtractor::addLandscapeToGltf(Document& gltfDoc, const LevelInternal& bodyInt, LotusLib::PackagesReader& pkgs)
{
	auto landscapeExtractor = Landscape::LandscapeExtractor::getInstance();

	LotusLib::PackageReader pkg = pkgs.getPackage("Misc").value();
	LotusLib::FileEntry landscapeEntry;
	try
	{
		landscapeEntry = pkg.getFile(bodyInt.landscape.landscapePath);
	}
	catch (std::exception& ex)
	{
		WarframeExporter::Logger::getInstance().error("Unable to find landscape " + bodyInt.landscape.landscapePath);
		return;
	}

	auto extHeader = landscapeExtractor->readHeader(&landscapeEntry.headerData, landscapeEntry.commonHeader);
    auto chunks = landscapeExtractor->readLandscapeChunks(&landscapeEntry.bData, extHeader, landscapeEntry.commonHeader);
    auto intLandscape = landscapeExtractor->formatLandscape(extHeader, chunks);

	size_t modelCountPre = gltfDoc.meshes.size();
	Landscape::LandscapeExporterGltf::addLandscapeChunks(gltfDoc, intLandscape);
	
	// Add level-specific attributes to each landscape chunk
	for (size_t modelIndex = modelCountPre; modelIndex < gltfDoc.meshes.size(); modelIndex++)
	{
		for (auto& attribute : bodyInt.landscape.attributes)
		{
			gltfDoc.meshes[modelIndex].extensionsAndExtras["extras"][attribute.first] = attribute.second;
		}
	}
}