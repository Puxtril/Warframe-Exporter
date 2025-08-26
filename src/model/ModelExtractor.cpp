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

void
ModelExtractor::extractExternal(LotusLib::FileEntry& fileEntry, LotusLib::Game game, ModelHeaderExternal& outHeaderExt, ModelBodyExternal& outBodyExt)
{
	if (fileEntry.bData.getLength() == 0)
		throw unknown_format_error("Mesh has no body");

	// Read body/header data
	ModelReader* modelReader = g_enumMapModel.at(game, fileEntry.commonHeader.type);

	modelReader->readHeader(&fileEntry.headerData, fileEntry.commonHeader, outHeaderExt);
	modelReader->readBody(outHeaderExt, &fileEntry.bData, outBodyExt);
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
	ModelHeaderExternal headerExt;
	ModelBodyExternal bodyExt;
	extractExternal(fileEntry, pkgs.getGame(), headerExt, bodyExt);

	if (headerExt.meshInfos.size() == 0)
		throw unknown_format_error("Mesh has zero MeshInfos");

	auto pkg = pkgs.getPackage("Misc");
	auto vertexColors = getVertexColors(fileEntry.internalPath, pkg.value(), options.extractVertexColors);

	// Convert body/header data into internal format
	ModelHeaderInternal headerInt;
	ModelBodyInternal bodyInt;
	ModelConverter::convertToInternal(headerExt, bodyExt, fileEntry.commonHeader.attributes, vertexColors, headerInt, bodyInt, g_enumMapModel.at(pkgs.getGame(), fileEntry.commonHeader.type)->ensmalleningScale());
	ModelConverter::mirrorX(headerInt, bodyInt);

	m_logger.debug(spdlog::fmt_lib::format("Bones={} Verts={} Faces={} Morphs={} PhysXMeshes={} Colors={} Scale={},{},{}", headerExt.boneTree.size(), headerExt.vertexCount, headerExt.faceCount, headerExt.morphCount, headerExt.physXMeshes.size(), vertexColors.size(), headerInt.modelScale.x, headerInt.modelScale.y, headerInt.modelScale.z, headerInt.modelScale.w));
	
	// Convert body/header into exportable format
	Document gltfDocument;
	ModelExporterGltf::addModelData(gltfDocument, headerInt, bodyInt, bodyExt);
	if (!options.dryRun)
		ModelExporterGltf::save(gltfDocument, outputPath);
}
