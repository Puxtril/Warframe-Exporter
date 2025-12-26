#include "model/ModelExtractor.h"

using namespace WarframeExporter::Model;

ModelExtractor*
ModelExtractor::getInstance()
{
	static ModelExtractor* instance = new ModelExtractor();
	return instance;
}

void
ModelExtractor::indexVertexColors(LotusLib::PackageReader& pkgs)
{
	m_vertexColorIndexer.indexVertexColors(pkgs);
}

void
ModelExtractor::cancelVertexColorIndexing()
{
	m_vertexColorIndexer.cancelIndexing();
}

ModelReader*
ModelExtractor::getModelReader(LotusLib::FileEntry& fileEntry, LotusLib::Game game)
{
	ModelReader* modelReader = nullptr;

	const std::string filename = fileEntry.internalPath.stem().string();
	if (strncmp(filename.c_str() + (filename.length() - 4), "_dcm", 4) == 0)
	{
		modelReader = g_enumMapModelDCM.at(game, fileEntry.commonHeader.type);
	}
	else if (strncmp(filename.c_str() + (filename.length() - 5), "_hlod", 5) == 0)
	{
		modelReader = g_enumMapModelHLOD.at(game, fileEntry.commonHeader.type);
	}
	else if (strncmp(filename.c_str() + (filename.length() - 3), "_sm", 3) == 0)
	{
		modelReader = g_enumMapModelSM.at(game, fileEntry.commonHeader.type);
	}
	else
	{
		modelReader = g_enumMapModel.at(game, fileEntry.commonHeader.type);
	}

	return modelReader;
}

void
ModelExtractor::extractExternal(ModelReader* modelReader, LotusLib::FileEntry& fileEntry, LotusLib::Game game, ModelHeaderExternal& outHeaderExt, ModelBodyExternal& outBodyExt)
{
	if (fileEntry.bData.getLength() == 0)
		throw unknown_format_error("Mesh has no body");

	modelReader->readHeader(&fileEntry.headerData, fileEntry.commonHeader, outHeaderExt);
	modelReader->readBody(outHeaderExt, &fileEntry.bData, &fileEntry.fData, outBodyExt);
}

std::vector<std::vector<glm::u8vec4>>
ModelExtractor::getVertexColors(const LotusLib::LotusPath& modelPath, LotusLib::PackageReader& pkg, bool indexVertexColors)
{
	std::vector<std::vector<glm::u8vec4>> vertexColors;
	if (indexVertexColors)
		m_vertexColorIndexer.getModelColors(modelPath, vertexColors, pkg);
	return vertexColors;
}

void
ModelExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options)
{
	ModelReader* modelReader = getModelReader(fileEntry, pkgs.getGame());
	if (modelReader == nullptr)
		throw unknown_format_error("Unable to match model " + std::to_string(fileEntry.commonHeader.type) + " with known format");
	
	ModelHeaderExternal headerExt;
	ModelBodyExternal bodyExt;
	extractExternal(modelReader, fileEntry, pkgs.getGame(), headerExt, bodyExt);

	if (headerExt.meshInfos.size() == 0)
		throw unknown_format_error("Mesh has zero MeshInfos");

	auto pkg = pkgs.getPackage("Misc");
	auto vertexColors = getVertexColors(fileEntry.internalPath, pkg.value(), options.extractVertexColors);

	// Convert body/header data into internal format
	ModelHeaderInternal headerInt;
	ModelBodyInternal bodyInt;
	ModelConverter::convertToInternal(headerExt, bodyExt, fileEntry.commonHeader.attributes, vertexColors, headerInt, bodyInt, modelReader->ensmalleningScale(), fileEntry.internalPath);
	ModelConverter::mirrorX(headerInt, bodyInt);

	m_logger.debug(spdlog::fmt_lib::format("Bones={} Verts={} Faces={} Morphs={} PhysXMeshes={} Colors={} Scale={},{},{}", headerExt.boneTree.size(), headerExt.vertexCount, headerExt.faceCount, headerExt.morphCount, headerExt.physXMeshes.size(), vertexColors.size(), headerInt.modelScale.x, headerInt.modelScale.y, headerInt.modelScale.z, headerInt.modelScale.w));
	
	// Convert body/header into exportable format
	Document gltfDocument;
	ModelExporterGltf::addModelData(gltfDocument, headerInt, bodyInt, bodyExt, fileEntry.internalPath);
	if (!options.dryRun)
		ModelExporterGltf::save(gltfDocument, outputPath);
}
