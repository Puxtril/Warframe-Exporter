#include "texture/ExtractorTexture.h"

using namespace WarframeExporter::Texture;

ExtractorTexture*
ExtractorTexture::getInstance()
{
	static ExtractorTexture* instance = new ExtractorTexture();
	return instance;
}

TextureHeaderInternal
ExtractorTexture::readHeader(BinaryReaderBuffered* HfileReader, BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header, Logger& logger)
{
	TextureHeaderExternal extHeader = TextureHeader::readHeader(HfileReader, ensmalleningData);
	size_t textureSize = FfileReader != nullptr ? FfileReader->getLength() : BfileReader->getLength();
	TextureHeaderInternal intHeader = TextureHeader::convertHeader(extHeader, textureSize);
	logger.debug(spdlog::fmt_lib::format("Raw texture data: Resolution={}x{} Enum1={} Enum2={} Enum3={}", extHeader.widthBase, extHeader.heightBase, extHeader.unkEnum1, extHeader.unkEnum2, extHeader.unkEnum3));
	logger.debug(spdlog::fmt_lib::format("Converted texture data: Resolution={}x{} Mip0Size={}", intHeader.width, intHeader.height, intHeader.mip0Len));
	return intHeader;
}

TextureBodyInternal
ExtractorTexture::readBody(BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header, const TextureHeaderInternal& headerInternal, Logger& logger)
{
	BinaryReaderBuffered* bodyReader = FfileReader != nullptr ? FfileReader : BfileReader;
	return TextureBody::getBody(bodyReader, headerInternal, ensmalleningData);
}

void
ExtractorTexture::writeData(const std::string& outputFile, const TextureHeaderInternal& header, const TextureBodyInternal& body, const CommonFileHeader& comHeader)
{
	std::ofstream out;
	out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
	
	static uint32_t magic = 542327876;
	ddspp::Header* ddsHeader = new ddspp::Header();
	ddspp::HeaderDXT10* ddsHeaderDX10 = new ddspp::HeaderDXT10();
	bool writeDXT10 = ddspp::encode_header(header.formatClass->format(), header.width, header.height, 1, ddspp::Texture2D, 1, 1, *ddsHeader, *ddsHeaderDX10);
	out.write((char*)&magic, 4);
	ddspp::serialize(out, *ddsHeader);
	if (writeDXT10)
		ddspp::serialize(out, *ddsHeaderDX10);

	int32_t mip0Start = body.dataLen - header.mip0Len;
	out.write(body.data + mip0Start, header.mip0Len);
	out.close();
}

void
ExtractorTexture::extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& pkgDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath)
{
	BinaryReaderBuffered* fReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::F, internalpath);
	BinaryReaderBuffered* bReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);

	TextureHeaderInternal headerInt = readHeader(hReader, fReader, bReader, ensmalleningData, header, m_logger);
	TextureBodyInternal body = readBody(fReader, bReader, ensmalleningData, header, headerInt, m_logger);
	writeData(outputPath, headerInt, body, header);
}

void
ExtractorTexture::extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& pkgDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData)
{
	BinaryReaderBuffered* fReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::F, internalpath);
	BinaryReaderBuffered* bReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);

	TextureHeaderInternal headerInt = readHeader(hReader, fReader, bReader, ensmalleningData, header, m_logger);
	TextureBodyInternal body = readBody(fReader, bReader, ensmalleningData, header, headerInt, m_logger);
}
