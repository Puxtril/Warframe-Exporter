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
LevelStaticExtractor::addModelsToGltf(LevelStaticExternal& external, LotusLib::PackagesReader& pkgs, fx::gltf::Document& gltf)
{
	LotusLib::PackageReader miscPkg = pkgs.getPackage("Misc").value();
	
	// Mimics the model array from the external LevelStatic header
	// Mapped to gltf Mesh indicies
	std::vector<ExporterGltf::ModelInfo> models(external.header.modelPaths.size());

	for (int i = 0; i < external.body.objects.size(); i++)
	{
		LevelStaticObjectExternal& curLevelObj = external.body.objects[i];

		// Too much crap
		if (external.header.modelPaths[curLevelObj.modelIndex] == "/EE/Editor/Darkitect/Objects/DeferredDecalProjector.fbx")
       		continue;

		// We have not extracted this model into the output gltf file
		if (models[curLevelObj.modelIndex].indexIndices.size() == 0)
		{
			LotusLib::FileEntry curLevelObjFile = miscPkg.getFile(external.header.modelPaths[curLevelObj.modelIndex]);

			if (WarframeExporter::Model::g_enumMapModel[curLevelObjFile.commonHeader.type] == nullptr)
			{
				m_logger.warn(spdlog::fmt_lib::format("Skipping unsupported type {}: {}", curLevelObjFile.commonHeader.type, external.header.modelPaths[curLevelObj.modelIndex]));
				continue;
			}

			WarframeExporter::Model::ModelHeaderExternal headerExt;
			WarframeExporter::Model::ModelBodyExternal bodyExt;
			WarframeExporter::Model::ModelExtractor::getInstance()->extractExternal(curLevelObjFile, headerExt, bodyExt);

			if (headerExt.meshInfos.size() == 0)
			{
				m_logger.debug(spdlog::fmt_lib::format("Skipping zero meshinfos {}", external.header.modelPaths[curLevelObj.modelIndex]));
				continue;
			}

			// Gltf exporter doesn't like multiple rigged models
			headerExt.boneTree.clear();

			WarframeExporter::Model::ModelHeaderInternal headerInt;
			WarframeExporter::Model::ModelBodyInternal bodyInt;
			auto vertexColors = WarframeExporter::Model::ModelExtractor::getInstance()->getVertexColors(external.header.modelPaths[curLevelObj.modelIndex], miscPkg);
			WarframeExporter::Model::ModelConverter::convertToInternal(headerExt, bodyExt, curLevelObjFile.commonHeader.attributes, vertexColors, headerInt, bodyInt, WarframeExporter::Model::g_enumMapModel[curLevelObjFile.commonHeader.type]->ensmalleningScale());
				
			models[curLevelObj.modelIndex] = ExporterGltf::addModel(gltf, headerInt, bodyInt, bodyExt);
		}

		ExporterGltf::addModelInstance(gltf, external.header, curLevelObj, models[curLevelObj.modelIndex]);
	}
}

void
LevelStaticExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, bool dryRun)
{
	LevelStaticExternal external = read(fileEntry);
	fx::gltf::Document gltf;
	addModelsToGltf(external, pkgs, gltf);
	if (!dryRun)
		fx::gltf::Save(gltf, outputPath, gltf.buffers.size() > 1 ? false : true);
}
