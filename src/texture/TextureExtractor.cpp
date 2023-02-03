#include "texture/TextureExtractor.h"

using namespace WarframeExporter::Texture;

TextureExtractor*
TextureExtractor::getInstance()
{
	static TextureExtractor* instance = new TextureExtractor();
	return instance;
}

void
TextureExtractor::writeData(const std::string& outputFile, const TextureHeaderInternal& header, const TextureBodyInternal& body, const CommonFileHeader& comHeader)
{
	std::ofstream out;
	out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
	
	DDSHeaderFull headerFull = DDSLib::encodeHeader(header.formatClass->getFormat(), header.width, header.height);
	DDSLib::serialize(out, headerFull);

	int32_t mip0Start = body.dataLen - header.mip0Len;
	out.write(body.data.get() + mip0Start, header.mip0Len);
	out.close();
}

void
TextureExtractor::extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& pkgDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath)
{
	BinaryReaderBuffered* fReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::F, internalpath);
	BinaryReaderBuffered* bReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);

	// Read header
	TextureHeaderExternal extHeader = TextureReader::readHeader(hReader, ensmalleningData);
	size_t textureSize = fReader != nullptr ? fReader->getLength() : bReader->getLength();
	TextureHeaderInternal headerInt = TextureConverter::convertHeader(extHeader, textureSize);

	m_logger.debug(spdlog::fmt_lib::format("Raw texture data: Format={} Resolution={}x{} Enum1={} Enum2={} Enum3={}", extHeader.format, extHeader.widthBase, extHeader.heightBase, extHeader.unkEnum1, extHeader.unkEnum2, extHeader.unkEnum3));
	m_logger.debug(spdlog::fmt_lib::format("Converted texture data: Resolution={}x{} Mip0Size={}", headerInt.width, headerInt.height, headerInt.mip0Len));

	// Read body
	BinaryReaderBuffered* bodyReader = fReader != nullptr ? fReader : bReader;
	TextureBodyInternal body = TextureReader::readBody(bodyReader, headerInt, ensmalleningData);

	writeData(outputPath, headerInt, body, header);
}

void
TextureExtractor::extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& pkgDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData)
{
	/*
	BinaryReaderBuffered* fReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::F, internalpath);
	BinaryReaderBuffered* bReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);

	TextureHeaderInternal headerInt = readHeader(hReader, fReader, bReader, ensmalleningData, header, m_logger);
	TextureBodyInternal body = readBody(fReader, bReader, ensmalleningData, header, headerInt, m_logger);
	*/
}
