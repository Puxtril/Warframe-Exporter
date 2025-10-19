#include "levelstatic/LevelStaticExtractor.h"

using namespace WarframeExporter::LevelStatic;

LevelStaticExtractor*
LevelStaticExtractor::getInstance()
{
	static LevelStaticExtractor* instance = new LevelStaticExtractor();
	return instance;
}

LevelStaticExternal
LevelStaticExtractor::read(LotusLib::FileEntry& fileEntry)
{
	LevelStaticReader* reader = g_enumMapLevelStatic[fileEntry.commonHeader.type];

	LevelStaticExternal external;
	reader->readHeader(&fileEntry.headerData, external.header);
	reader->readBody(&fileEntry.bData, external.header, external.body);

	return external;
}

void
LevelStaticExtractor::addModelsToGltf(LevelStaticExternal& external, LotusLib::PackagesReader& pkgs, fx::gltf::Document& gltf, ExtractOptions options)
{
	std::unordered_map<std::string, std::vector<int32_t>> modelPathsInGltf;
	addModelsToGltf(external, pkgs, gltf, modelPathsInGltf, options);
}

void
LevelStaticExtractor::addModelsToGltf(LevelStaticExternal& external, LotusLib::PackagesReader& pkgs, fx::gltf::Document& gltf, std::unordered_map<std::string, std::vector<int32_t>> modelPathsInGltf, ExtractOptions options)
{
	LotusLib::PackageReader miscPkg = pkgs.getPackage("Misc").value();
	
	for (int i = 0; i < external.body.objects.size(); i++)
	{
		LevelStaticObjectExternal& curLevelObj = external.body.objects[i];
		const std::string& curMeshPath = external.header.modelPaths[curLevelObj.modelIndex];

		// Too much crap
		if (curMeshPath == "/EE/Editor/Darkitect/Objects/DeferredDecalProjector.fbx")
       		continue;

		// We have not extracted this model into the output gltf file
		if (modelPathsInGltf.count(curMeshPath) == 0)
		{
			LotusLib::FileEntry curLevelObjFile = miscPkg.getFile(curMeshPath);

			if (curLevelObjFile.headerData.getLength() == 0)
			{
				m_logger.warn(spdlog::fmt_lib::format("Object doesn't exist: {}", curMeshPath));
				continue;
			}

			Model::ModelReader* modelReader = Model::ModelExtractor::getInstance()->getModelReader(curLevelObjFile, pkgs.getGame());
			if (modelReader == nullptr)
			{
				m_logger.warn(spdlog::fmt_lib::format("Skipping unsupported type {}: {}", curLevelObjFile.commonHeader.type, curMeshPath));
				continue;
			}

			WarframeExporter::Model::ModelHeaderExternal headerExt;
			WarframeExporter::Model::ModelBodyExternal bodyExt;
			WarframeExporter::Model::ModelExtractor::getInstance()->extractExternal(modelReader, curLevelObjFile, pkgs.getGame(), headerExt, bodyExt);

			if (headerExt.meshInfos.size() == 0)
			{
				m_logger.debug(spdlog::fmt_lib::format("Skipping zero meshinfos {}", curMeshPath));
				continue;
			}

			// Gltf exporter doesn't like multiple rigged models
			headerExt.boneTree.clear();

			WarframeExporter::Model::ModelHeaderInternal headerInt;
			WarframeExporter::Model::ModelBodyInternal bodyInt;
			auto vertexColors = WarframeExporter::Model::ModelExtractor::getInstance()->getVertexColors(curMeshPath, miscPkg, options.extractVertexColors);
			WarframeExporter::Model::ModelConverter::convertToInternal(headerExt, bodyExt, curLevelObjFile.commonHeader.attributes, vertexColors, headerInt, bodyInt, modelReader->ensmalleningScale(), curMeshPath);
				
			modelPathsInGltf[curMeshPath] = ExporterGltf::addModel(gltf, headerInt, bodyInt, bodyExt);
		}

		ExporterGltf::addModelInstance(gltf, external.header, curLevelObj, modelPathsInGltf[curMeshPath]);
	}
}

void
LevelStaticExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options)
{
	LevelStaticExternal external = read(fileEntry);
	fx::gltf::Document gltf;
	addModelsToGltf(external, pkgs, gltf, options);
	if (!options.dryRun)
		fx::gltf::Save(gltf, outputPath, gltf.buffers.size() > 1 ? false : true);
}
