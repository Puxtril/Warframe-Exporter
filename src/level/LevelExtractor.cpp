#include "level/LevelExtractor.h"

using namespace WarframeExporter::Level;

LevelExtractor*
LevelExtractor::getInstance()
{
	static LevelExtractor* instance = new LevelExtractor();
	return instance;
}

LevelExternal
LevelExtractor::getLevelExternal(uint32_t fileType, BinaryReader::Buffered& headerData, BinaryReader::Buffered& bodyData)
{
	LevelReader* levelReader = g_enumMapLevel[fileType];
	
	LevelExternal external;
	external.landscapeIndex = -1;
	levelReader->readHeader(headerData, external.header);
	levelReader->readBody(bodyData, external.header, external.body);

	return external;
}

LevelInternal
LevelExtractor::convertToInternal(const std::string& internalPath, LevelExternal& levelExternal)
{
	LevelInternal bodyInt;
	LevelConverter::convertToInternal(levelExternal.header, levelExternal.body, internalPath, bodyInt);
	return bodyInt;
}

void
LevelExtractor::findExtraAttributes(const std::string& pkgsbinAttributes, LevelExternal& levelExternal)
{
	for (size_t i = 0; i < levelExternal.header.levelObjs.size(); i++)
	{
		std::vector<char>& curAttributes = levelExternal.body.attributes[i];

		if (pkgsbinAttributes.size() > curAttributes.size())
		{
			size_t oldSize = curAttributes.size();
			curAttributes.resize(oldSize + pkgsbinAttributes.length());
			memcpy(&curAttributes[oldSize-1], pkgsbinAttributes.c_str(), pkgsbinAttributes.length());
		}
	}
}

void
LevelExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	const std::string& internalPath = LotusLib::getFullPath(fileEntry.headerNode);

	LevelExternal levelExt = getLevelExternal(fileEntry.commonHeader.type, fileEntry.header, fileEntry.body);
	findExtraAttributes(pkgsBin.getParameters(internalPath), levelExt);
	LevelInternal levelInt = convertToInternal(internalPath, levelExt);

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
LevelExtractor::createGltfCombined(const LotusLib::PackageCollection& pkgs, LevelInternal& bodyInt, ExtractOptions options)
{
	Document outGltf;

	LotusLib::Package miscPkg = pkgs.getPackage("Misc");

	for (size_t x = 0; x < bodyInt.objs.size(); x++)
	{
		// >3GB
		if (outGltf.buffers.size() > 0 && outGltf.buffers.back().data.size() > 3221225472)
			outGltf.buffers.resize(outGltf.buffers.size() + 1);

		const LevelObjectInternal& curLevelObj = bodyInt.objs[x];

		if (curLevelObj.meshPath == "" && curLevelObj.objTypePath != "/EE/Types/Effects/Landscape")
			continue;

		bool isHlod = curLevelObj.objTypePath.length() >= 19 && curLevelObj.objTypePath.compare(curLevelObj.objTypePath.length() - 19, 19, "HLODAggregateEntity") == 0;
		if (
			(isHlod && options.levelHlodExtractMode == LevelHlodExtractMode::IGNORE_HLOD) ||
			(!isHlod && options.levelHlodExtractMode == LevelHlodExtractMode::ONLY_HLOD)
		) { continue; }

		try
		{
			if (curLevelObj.objTypePath == "/EE/Types/Effects/Landscape" && options.includeLandscapeInLevel)
				addLandscapeToGltf(curLevelObj, bodyInt, pkgs, outGltf);
			else
				addModelToGltf(curLevelObj, options.extractVertexColors, pkgs, outGltf);
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
LevelExtractor::addModelToGltf(const LevelObjectInternal& levelObj, bool extractVertexColors, const LotusLib::PackageCollection& pkgs, Document& gltfDoc)
{
	LotusLib::Package miscPkg = pkgs.getPackage("Misc");
	LotusLib::FileEntry curLevelEntry = miscPkg.getFileEntry(levelObj.meshPath);

	if (curLevelEntry.header.getLength() == 0)
	{
		m_logger.warn(spdlog::fmt_lib::format("Object doesn't exist: {}", levelObj.meshPath));
		return;
	}

	Model::ModelReader* modelReader = Model::ModelExtractor::getInstance()->getModelReader(levelObj.meshPath, curLevelEntry.commonHeader.type, pkgs.getGame());
	if (modelReader == nullptr)
	{
		m_logger.warn(spdlog::fmt_lib::format("Skipping unsupported type {}: {}", curLevelEntry.commonHeader.type, levelObj.meshPath));
		return;
	}

	WarframeExporter::Model::ModelHeaderExternal headerExt;
	WarframeExporter::Model::ModelBodyExternal bodyExt;
	WarframeExporter::Model::ModelExtractor::getInstance()->extractExternal(modelReader, curLevelEntry.commonHeader, &curLevelEntry.header, &curLevelEntry.body, &curLevelEntry.footer, pkgs.getGame(), headerExt, bodyExt);

	if (headerExt.meshInfos.size() == 0)
		return;

	// Gltf exporter doesn't like multiple rigged models
	headerExt.boneTree.clear();

	WarframeExporter::Model::ModelHeaderInternal headerInt;
	WarframeExporter::Model::ModelBodyInternal bodyInt;
	auto vertexColors = WarframeExporter::Model::ModelExtractor::getInstance()->getVertexColors(levelObj.meshPath, miscPkg, extractVertexColors);
	WarframeExporter::Model::ModelConverter::convertToInternal(headerExt, bodyExt, curLevelEntry.commonHeader.attributes, vertexColors, headerInt, bodyInt, modelReader->ensmalleningScale(), levelObj.meshPath);

	LevelConverter::replaceOverrideMaterials(levelObj.materials, headerInt);

	LevelExporterGltf::addModel(gltfDoc, headerInt, bodyInt, bodyExt, levelObj);
}

void
LevelExtractor::addLandscapeToGltf(const LevelObjectInternal& levelObj, const LevelInternal& bodyInt, const LotusLib::PackageCollection& pkgs, Document& gltfDoc)
{
	auto landscapeExtractor = Landscape::LandscapeExtractor::getInstance();

	const std::string landscapePath = levelObj.attributes.at("DataRes").get<std::string>();

	if (!pkgs.getPackage("Misc").fileExists(landscapePath))
	{
		WarframeExporter::Logger::getInstance().error("Unable to find landscape " + landscapePath);
		return;
	}

	LotusLib::Package pkg = pkgs.getPackage("Misc");
	LotusLib::FileEntry landscapeEntry = pkg.getFileEntry(landscapePath);

	auto extHeader = landscapeExtractor->readHeader(&landscapeEntry.header, landscapeEntry.commonHeader);
    auto chunks = landscapeExtractor->readLandscapeChunks(&landscapeEntry.body, extHeader, landscapeEntry.commonHeader);
    auto intLandscape = landscapeExtractor->formatLandscape(extHeader, chunks);

	size_t modelCountPre = gltfDoc.meshes.size();
	Landscape::LandscapeExporterGltf::addLandscapeChunks(gltfDoc, intLandscape, {levelObj.pos.x, 0, levelObj.pos.z});
	
	// Add level-specific attributes to each landscape chunk
	for (size_t modelIndex = modelCountPre; modelIndex < gltfDoc.meshes.size(); modelIndex++)
	{
		for (auto& attribute : levelObj.attributes)
		{
			gltfDoc.meshes[modelIndex].extensionsAndExtras["extras"][attribute.first] = attribute.second;
		}
	}
}