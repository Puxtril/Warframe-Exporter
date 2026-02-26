#include "model/ModelExtractor.h"

using namespace WarframeExporter::Model;

ModelExtractor*
ModelExtractor::getInstance()
{
	static ModelExtractor* instance = new ModelExtractor();
	return instance;
}

void
ModelExtractor::indexVertexColors(LotusLib::Package& pkgs)
{
	m_vertexColorIndexer.indexVertexColors(pkgs);
}

void
ModelExtractor::cancelVertexColorIndexing()
{
	m_vertexColorIndexer.cancelIndexing();
}

ModelReader*
ModelExtractor::getModelReader(const std::string& filename, uint32_t type, LotusLib::Game game)
{
	ModelReader* modelReader = nullptr;

	// Strip file extension
	const std::string name = std::filesystem::path(filename).stem().string();

	if (strncmp(name.c_str() + (name.length() - 4), "_dcm", 4) == 0)
	{
		modelReader = g_enumMapModelDCM.at(game, type);
	}
	else if (strncmp(name.c_str() + (name.length() - 5), "_hlod", 5) == 0)
	{
		modelReader = g_enumMapModelHLOD.at(game, type);
	}
	else if (strncmp(name.c_str() + (name.length() - 3), "_sm", 3) == 0)
	{
		modelReader = g_enumMapModelSM.at(game, type);
	}
	else
	{
		modelReader = g_enumMapModel.at(game, type);
	}

	return modelReader;
}

void
ModelExtractor::extractExternal(
	ModelReader* modelReader,
	LotusLib::CommonHeader& commonHeader,
	BinaryReader::Buffered* headerReader,
	BinaryReader::Buffered* bodyReader,
	BinaryReader::Buffered* footerReader,
	LotusLib::Game game,
	ModelHeaderExternal& outHeaderExt,
	ModelBodyExternal& outBodyExt
)
{
	if (bodyReader->getLength() == 0)
		throw unknown_format_error("Mesh has no body");

	modelReader->readHeader(headerReader, commonHeader, outHeaderExt);
	modelReader->readBody(outHeaderExt, bodyReader, footerReader, outBodyExt);
}

std::vector<std::vector<glm::u8vec4>>
ModelExtractor::getVertexColors(const std::string& modelPath, LotusLib::Package& pkg, bool indexVertexColors)
{
	std::vector<std::vector<glm::u8vec4>> vertexColors;
	if (indexVertexColors)
		m_vertexColorIndexer.getModelColors(modelPath, vertexColors, pkg);
	return vertexColors;
}

void
ModelExtractor::extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	ModelReader* modelReader = getModelReader(fileEntry.headerNode.name, fileEntry.commonHeader.type, pkgs.getGame());
	if (modelReader == nullptr)
		throw unknown_format_error("Unable to match model " + std::to_string(fileEntry.commonHeader.type) + " with known format");
	
	ModelHeaderExternal headerExt;
	ModelBodyExternal bodyExt;
	extractExternal(modelReader, fileEntry.commonHeader, &fileEntry.header, &fileEntry.body, &fileEntry.footer, pkgs.getGame(), headerExt, bodyExt);

	if (headerExt.meshInfos.size() == 0)
		throw unknown_format_error("Mesh has zero MeshInfos");

	const std::string fullInternalPath = LotusLib::getFullPath(fileEntry.headerNode);
	auto pkg = pkgs.getPackage("Misc");
	auto vertexColors = getVertexColors(fullInternalPath, pkg, options.extractVertexColors);

	// Convert body/header data into internal format
	ModelHeaderInternal headerInt;
	ModelBodyInternal bodyInt;
	ModelConverter::convertToInternal(headerExt, bodyExt, fileEntry.commonHeader.attributes, vertexColors, headerInt, bodyInt, modelReader->ensmalleningScale(), fullInternalPath);
	ModelConverter::mirrorX(headerInt, bodyInt);

	m_logger.debug(spdlog::fmt_lib::format("Bones={} Verts={} Faces={} Morphs={} PhysXMeshes={} Colors={} Scale={},{},{}", headerExt.boneTree.size(), headerExt.vertexCount, headerExt.faceCount, headerExt.morphCount, headerExt.physXMeshes.size(), vertexColors.size(), headerInt.modelScale.x, headerInt.modelScale.y, headerInt.modelScale.z, headerInt.modelScale.w));
	
	// Convert body/header into exportable format
	Document gltfDocument;
	ModelExporterGltf::addModelData(gltfDocument, headerInt, bodyInt, bodyExt, fullInternalPath);
	if (!options.dryRun)
		ModelExporterGltf::save(gltfDocument, outputPath);
}
