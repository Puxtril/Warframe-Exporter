#include "model/ModelExtractor.h"

using namespace WarframeExporter::Model;

ModelExtractor*
ModelExtractor::getInstance()
{
	static ModelExtractor* instance = new ModelExtractor();
	return instance;
}

void
ModelExtractor::extractExternal(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, ModelHeaderExternal& outHeaderExt, ModelBodyExternal& outBodyExt)
{
	const LotusLib::FileEntries::FileNode* bEntry = pkgDir[package][LotusLib::PackageTrioType::B]->getFileEntry(internalPath);
	std::unique_ptr<char[]> bRawData = pkgDir[package][LotusLib::PackageTrioType::B]->getDataAndDecompress(bEntry);
	BinaryReaderBuffered bReader = BinaryReaderBuffered((uint8_t*)bRawData.release(), bEntry->getLen());

	if (bEntry == nullptr)
		throw unknown_format_error("Mesh has no body");

	// Read body/header data
	ModelReader* modelReader = g_enumMapModel[header.type];

	modelReader->readHeader(hReader, ensmalleningData, header, outHeaderExt);
	modelReader->readBody(outHeaderExt, &bReader, outBodyExt);

	m_logger.debug(spdlog::fmt_lib::format("Raw model data: Bones={} WeightedBones={} Submeshes={} Vertices={} Faces={} Morphs={} PhysXMeshes={}", outHeaderExt.boneTree.size(), outHeaderExt.weightedBoneNames.size(), outHeaderExt.meshInfos.size(), outHeaderExt.vertexCount, outHeaderExt.faceCount, outHeaderExt.morphCount, outHeaderExt.physXMeshes.size()));

	// Error checking. Properly extracted models will never trigger this.
	if (outHeaderExt.meshInfos.size() == 0)
		throw unknown_format_error("Mesh has zero MeshInfos");
}

void
ModelExtractor::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	ModelHeaderExternal headerExt;
	ModelBodyExternal bodyExt;
	extractExternal(header, hReader, pkgDir, package, internalPath, ensmalleningData, headerExt, bodyExt);

	std::vector<std::vector<glm::u8vec4>> vertexColors;
	if (m_indexVertexColors)
		m_vertexColorIndexer.getModelColors(internalPath, vertexColors, pkgDir["Misc"]);

	// Convert body/header data into internal format
	ModelHeaderInternal headerInt;
	ModelBodyInternal bodyInt;
	ModelConverter::convertToInternal(headerExt, bodyExt, header.attributes, vertexColors, headerInt, bodyInt);
	m_logger.debug(spdlog::fmt_lib::format("Converted model data: Scale={},{},{}", headerInt.modelScale.x, headerInt.modelScale.y, headerInt.modelScale.z));
	
	// Convert body/header into exportable format
	ModelExporterGltf outModel;
	outModel.addModelData(headerInt, bodyInt);
	outModel.save(outputPath);
}

void
ModelExtractor::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData)
{
	const LotusLib::FileEntries::FileNode* bEntry = pkgDir[package][LotusLib::PackageTrioType::B]->getFileEntry(internalPath);
	std::unique_ptr<char[]> bRawData = pkgDir[package][LotusLib::PackageTrioType::B]->getDataAndDecompress(bEntry);
	BinaryReaderBuffered bReader = BinaryReaderBuffered((uint8_t*)bRawData.release(), bEntry->getLen());

	ModelHeaderExternal headerExt;

	size_t pos = hReader->tell();
	ModelReader* modelReader = g_enumMapModel[header.type];

	modelReader->readHeaderDebug(hReader, ensmalleningData, header);
	hReader->seek(pos, std::ios_base::beg);
	modelReader->readHeader(hReader, ensmalleningData, header, headerExt);
	if (headerExt.vertexCount > 0)
		modelReader->readBodyDebug(headerExt, &bReader);
}
