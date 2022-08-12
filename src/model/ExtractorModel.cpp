#include "model/ExtractorModel.h"

using namespace WarframeExporter::Model;

ExtractorModel&
ExtractorModel::getInstance()
{
	static ExtractorModel instance;
	return instance;
}

void
ExtractorModel::extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, BinaryReaderBuffered* bReader, BinaryReaderBuffered* fReader, const Ensmallening& ensmalleningData, const std::string& outputPath)
{
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

	if (headerExt.getMeshInfos().size() == 0)
		throw InvalidDataException("Mesh has zero MeshInfos");

	ModelHeaderInternal headerInt;
	ModelBodyInternal bodyInt;
	ModelConverter::convertToInternal(headerExt, bodyExt, header.getAttributes(), headerInt, bodyInt);
	
	GltfModel outModel;
	outModel.addModelData(headerInt, bodyInt);
	outModel.save(outputPath);
}

void
ExtractorModel::extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, BinaryReaderBuffered* bReader, BinaryReaderBuffered* fReader, const Ensmallening& ensmalleningData)
{
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