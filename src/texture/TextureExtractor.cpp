#include "texture/TextureExtractor.h"

using namespace WarframeExporter::Texture;

TextureExtractor*
TextureExtractor::getInstance()
{
	static TextureExtractor* instance = new TextureExtractor();
	return instance;
}

void
TextureExtractor::writeData(const std::filesystem::path& outputFile, const TextureHeaderInternal& header, const TextureBodyInternal& body, const LotusLib::CommonHeader& comHeader)
{
	std::ofstream out;
	out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
	
	DDSHeaderFull headerFull = DDSLib::encodeHeader(header.formatClass->getFormat(), header.width, header.height);
	DDSLib::serialize(out, headerFull);

	uint32_t mip0Start = (uint32_t)body.dataLen - header.mip0Len;
	out.write(body.data.get() + mip0Start, header.mip0Len);
	out.close();
}

void
TextureExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	// Typically, textures above 256x256 are in F. Textures below are in B
	BinaryReader::BinaryReaderBuffered& entry = fileEntry.fData.getLength() != 0 ? fileEntry.fData : fileEntry.bData;

	// Read header
	TextureHeaderExternal extHeader = TextureReader::readHeader(&fileEntry.headerData, ensmalleningData);
	TextureHeaderInternal headerInt = TextureConverter::convertHeader(extHeader, entry.getLength());

	m_logger.debug(spdlog::fmt_lib::format("Format={} ResRaw={}x{} ResConv={}x{} Mip0Size={}", extHeader.format, extHeader.widthBase, extHeader.heightBase, headerInt.width, headerInt.height, headerInt.mip0Len));

	// Read body
	TextureBodyInternal body = TextureReader::readBody(&entry, headerInt, ensmalleningData);

	writeData(outputPath, headerInt, body, fileEntry.commonHeader);
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
