#include "model/ExtractorModel.h"

using namespace WarframeExporter::Model;

ExtractorModel*
ExtractorModel::getInstance()
{
	static ExtractorModel* instance = new ExtractorModel();
	return instance;
}

void
ExtractorModel::extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath)
{
	BinaryReaderBuffered* bReader = cacheDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);

	if (bReader->getLength() == 0)
		throw InvalidDataException("Mesh has no body");

	// Read body/header data
	WFModelReader* modelReader = g_enumMapModel[header.getEnum()];

	ModelHeaderExternal headerExt;
	modelReader->readHeader(hReader, ensmalleningData, header, headerExt);

	ModelBodyExternal bodyExt;
	modelReader->readBody(headerExt, bReader, bodyExt);

	m_logger.debug(spdlog::fmt_lib::format("Raw model data: Bones={} WeightedBones={} Submeshes={} Vertices={} Faces={} Morphs={} PhysXMeshes={}", headerExt.boneTree.size(), headerExt.weightedBoneNames.size(), headerExt.meshInfos.size(), headerExt.vertexCount, headerExt.faceCount, headerExt.morphCount, headerExt.physXMeshes.size()));

	// Error checking. Properly extracted models will never trigger this.
	if (headerExt.meshInfos.size() == 0)
		throw InvalidDataException("Mesh has zero MeshInfos");

	// Convert body/header data into internal format
	ModelHeaderInternal headerInt;
	ModelBodyInternal bodyInt;
	ModelConverter::convertToInternal(headerExt, bodyExt, header.getAttributes(), headerInt, bodyInt);
	m_logger.debug(spdlog::fmt_lib::format("Converted model data: Scale={},{},{}", headerInt.modelScale.x, headerInt.modelScale.y, headerInt.modelScale.z));
	
	// Convert body/header into exportable format
	GltfModel outModel;
	outModel.addModelData(headerInt, bodyInt);
	outModel.save(outputPath);
}

void
ExtractorModel::extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData)
{
	BinaryReaderBuffered* bReader = cacheDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);
	ModelHeaderExternal headerExt;

	size_t pos = hReader->tell();
	WFModelReader* modelReader = g_enumMapModel[header.getEnum()];

	modelReader->readHeaderDebug(hReader, ensmalleningData, header);
	hReader->seek(pos, std::ios_base::beg);
	modelReader->readHeader(hReader, ensmalleningData, header, headerExt);
	modelReader->readBodyDebug(headerExt, bReader);
}
