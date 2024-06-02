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
	LevelObjectHeaderExternal& landscape = external.header.levelObjs[external.landscapeIndex];

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
LevelExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	LevelExternal levelExt = getLevelExternal(fileEntry);
	LevelInternal levelInt = convertToInternal(fileEntry, levelExt);

	m_logger.info("Level mesh count: " + std::to_string(levelInt.objs.size()));

	Document gltfOut = createGltfCombined(pkgs, ensmalleningData, levelInt);
	Model::ModelExporterGltf::save(gltfOut, outputPath);
}

Document
LevelExtractor::createGltfCombined(LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, LevelInternal& bodyInt)
{
	Document outGltf;

	LotusLib::PackageReader miscPkg = pkgs.getPackage("Misc");

	if (!bodyInt.landscape.landscapePath.empty())
		addLandscapeToGltf(outGltf, bodyInt, pkgs);

	for (size_t x = 0; x < bodyInt.objs.size(); x++)
	{
		LevelObjectInternal& curLevelObj = bodyInt.objs[x];

		if (curLevelObj.meshPath == "")
			continue;

		try
		{
			LotusLib::FileEntry curLevelObjFile = miscPkg.getFile(curLevelObj.meshPath);

			if (WarframeExporter::Model::g_enumMapModel[curLevelObjFile.commonHeader.type] == nullptr)
			{
				m_logger.warn(spdlog::fmt_lib::format("Skipping unsupported type {}: {}", curLevelObjFile.commonHeader.type, curLevelObj.meshPath));
				continue;
			}

			WarframeExporter::Model::ModelHeaderExternal headerExt;
			WarframeExporter::Model::ModelBodyExternal bodyExt;
			WarframeExporter::Model::ModelExtractor::getInstance()->extractExternal(curLevelObjFile, ensmalleningData, headerExt, bodyExt);

			if (headerExt.meshInfos.size() == 0)
			{
				m_logger.debug(spdlog::fmt_lib::format("Skipping zero meshinfos {}", curLevelObj.meshPath));
				continue;
			}

			// Gltf exporter doesn't like multiple rigged models
			headerExt.boneTree.clear();

			WarframeExporter::Model::ModelHeaderInternal headerInt;
			WarframeExporter::Model::ModelBodyInternal bodyInt;
			auto vertexColors = WarframeExporter::Model::ModelExtractor::getInstance()->getVertexColors(curLevelObj.meshPath, miscPkg);
			WarframeExporter::Model::ModelConverter::convertToInternal(headerExt, bodyExt, curLevelObjFile.commonHeader.attributes, vertexColors, headerInt, bodyInt, WarframeExporter::Model::g_enumMapModel[curLevelObjFile.commonHeader.type]->ensmalleningScale());

			LevelConverter::applyTransformation(curLevelObj, bodyInt.positions);
			LevelConverter::replaceOverrideMaterials(curLevelObj.materials, headerInt);

			m_logger.debug(spdlog::fmt_lib::format("Pos={},{},{} Scale={},{},{} Colors={} {}", curLevelObj.pos.x, curLevelObj.pos.y, curLevelObj.pos.z, headerInt.modelScale.x, headerInt.modelScale.y, headerInt.modelScale.z, vertexColors.size(), curLevelObj.meshPath));

			LevelExporterGltf::addModelData(outGltf, headerInt, bodyInt, curLevelObj);
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

	LotusLib::PackageReader pkg = pkgs.getPackage("Misc");
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