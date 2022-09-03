#include "model/ExtractorModel.h"

using namespace WarframeExporter::Model;

ExtractorModel&
ExtractorModel::getInstance()
{
	static ExtractorModel instance;
	return instance;
}

void
ExtractorModel::extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath)
{
	BinaryReaderBuffered* bReader = cacheDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);

	if (bReader->getLength() == 0)
		throw InvalidDataException("Mesh has no body");

	ModelHeaderExternal headerExt;
	ModelBodyExternal bodyExt;

	switch (header.getEnum())
	{
	case 269:
		ModelReader269::readHeader(hReader, ensmalleningData, header, headerExt);
		ModelReader269::readBody(headerExt, bReader, bodyExt);
		break;
	case 96:
		ModelReader96::readHeader(hReader, ensmalleningData, header, headerExt);
		ModelReader96::readBody(headerExt, bReader, bodyExt);
		break;
	}
	m_logger->debug(spdlog::fmt_lib::format("Raw model data: Bones={} WeightedBones={} Submeshes={} Vertices={} Faces={} Morphs={} PhysXMeshes={}", headerExt.getBoneTree().size(), headerExt.getWeightedBoneNames().size(), headerExt.getMeshInfos().size(), headerExt.getVertexCount(), headerExt.getFaceCount(), headerExt.getMorphCount(), headerExt.getPhysXMeshes().size()));

	if (headerExt.getMeshInfos().size() == 0)
		throw InvalidDataException("Mesh has zero MeshInfos");

	std::vector<VertexColorBody> vertexColors = VertexColorManager::getInstance().getModelColors(internalpath);
	m_logger->debug(spdlog::fmt_lib::format("Found {} vertex colors", vertexColors.size()));

	ModelHeaderInternal headerInt;
	ModelBodyInternal bodyInt;
	ModelConverter::convertToInternal(headerExt, bodyExt, header.getAttributes(), vertexColors, headerInt, bodyInt);
	m_logger->debug(spdlog::fmt_lib::format("Converted model data: Scale={},{},{}", headerInt.getModelScale().x, headerInt.getModelScale().y, headerInt.getModelScale().z));
	
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
	switch (header.getEnum())
	{
	case 269:
		ModelReader269::readHeaderDebug(hReader, ensmalleningData, header);
		hReader->seek(pos, std::ios_base::beg);
		ModelReader269::readHeader(hReader, ensmalleningData, header, headerExt);
		ModelReader269::readBodyDebug(headerExt, bReader);
		break;
	case 96:
		ModelReader96::readHeaderDebug(hReader, ensmalleningData, header);
		hReader->seek(pos, std::ios_base::beg);
		ModelReader96::readHeader(hReader, ensmalleningData, header, headerExt);
		ModelReader96::readBodyDebug(headerExt, bReader);
		break;
	}
}