#include "texture/TextureExtractor.h"

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

	TextureInternal intTexture;
	intTexture.header = TextureConverter::convertHeader(extHeader, entry.getLength());

	m_logger.debug(spdlog::fmt_lib::format("Format={} ResRaw={}x{} ResConv={}x{} Mip0Size={}", extHeader.format, extHeader.widthBase, extHeader.heightBase, intTexture.header.width, intTexture.header.height, intTexture.header.mip0Len));

	// Read body
	intTexture.body = TextureReader::readBody(&entry, intTexture.header, ensmalleningData);

	return intTexture;
}

void
TextureExtractor::writeData(TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const std::filesystem::path& outputFile)
{
	std::ofstream out;
	out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
	
	DDSHeaderFull headerFull = DDSLib::encodeHeader(texture.header.formatClass->getFormat(), texture.header.width, texture.header.height);
	DDSLib::serialize(out, headerFull);

	uint32_t mip0Start = (uint32_t)texture.body.size() - texture.header.mip0Len;
	out.write(texture.body.data() + mip0Start, texture.header.mip0Len);
	out.close();
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
