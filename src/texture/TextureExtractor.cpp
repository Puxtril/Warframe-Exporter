#include "texture/TextureExtractor.h"
#include <stdexcept>

using namespace WarframeExporter::Texture;

TextureExtractor*
TextureExtractor::getInstance()
{
	static TextureExtractor* instance = new TextureExtractor();
	return instance;
}

TextureInternal
TextureExtractor::getTexture(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData)
{
	// Typically, textures above 256x256 are in F. Textures below are in B
	BinaryReader::BinaryReaderBuffered& entry = fileEntry.fData.getLength() != 0 ? fileEntry.fData : fileEntry.bData;

	// Read header
	TextureHeaderExternal extHeader = TextureReader::readHeader(&fileEntry.headerData, ensmalleningData);

	if (extHeader.format == (uint8_t)TextureCompression::BC6)
		throw std::runtime_error("BC6 textures currently unsupported");

	if (g_enumMapTexture[(int)extHeader.format] == nullptr)
		throw std::runtime_error("Unknown texture compression format: " + std::to_string(extHeader.format));

	TextureInternal intTexture;
	intTexture.header = TextureConverter::convertHeader(extHeader, static_cast<int32_t>(entry.getLength()));

	m_logger.debug(spdlog::fmt_lib::format("Format={} ResRaw={}x{} ResConv={}x{} Mip0Size={}", extHeader.format, extHeader.widthBase, extHeader.heightBase, intTexture.header.width, intTexture.header.height, intTexture.header.mip0Len));

	// Read body
	intTexture.body = TextureReader::readBody(&entry, intTexture.header, ensmalleningData);

	return intTexture;
}

void
TextureExtractor::writeData(TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const std::filesystem::path& outputFile)
{
	uint32_t mip0Start = (uint32_t)texture.body.size() - texture.header.mip0Len;
	char* dataStart = texture.body.data() + mip0Start;
	size_t dataLen = texture.header.mip0Len;

	if (TextureExtractor::m_exportType == TextureExportType::TEXTURE_EXPORT_DDS)
	{
		std::ofstream out;
		out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
		
		DDSHeaderFull headerFull = DDSLib::encodeHeader(texture.header.formatClass->getFormat(), texture.header.width, texture.header.height);
		DDSLib::serialize(out, headerFull);

		out.write(dataStart, dataLen);
		out.close();
	}

	else if (texture.header.formatEnum == TextureCompression::BC6)
		TextureExporterConvert::convertAndWriteToHdr(dataStart, dataLen, outputFile, texture.header.width, texture.header.height);

	else if (TextureExtractor::m_exportType == TextureExportType::TEXTURE_EXPORT_PNG)
		TextureExporterConvert::convertAndWriteToPng(dataStart, dataLen, outputFile, texture.header.formatEnum, texture.header.width, texture.header.height);
	 
	else if (TextureExtractor::m_exportType == TextureExportType::TEXTURE_EXPORT_TGA)
		TextureExporterConvert::convertAndWriteToTga(dataStart, dataLen, outputFile, texture.header.formatEnum, texture.header.width, texture.header.height);
}

void
TextureExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	TextureInternal intTexture = getTexture(fileEntry, pkgs, ensmalleningData);
	writeData(intTexture, fileEntry.commonHeader, outputPath);
}

void
TextureExtractor::extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData)
{
	/*
	BinaryReaderBuffered* fReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::F, internalpath);
	BinaryReaderBuffered* bReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);

	TextureHeaderInternal headerInt = readHeader(hReader, fReader, bReader, ensmalleningData, header, m_logger);
	TextureBodyInternal body = readBody(fReader, bReader, ensmalleningData, header, headerInt, m_logger);
	*/
}
