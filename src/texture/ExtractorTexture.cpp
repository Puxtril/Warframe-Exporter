#include "texture/ExtractorTexture.h"

using namespace WarframeExporter::Texture;

ExtractorTexture&
ExtractorTexture::getInstance()
{
	static ExtractorTexture instance;
	return instance;
}

TextureHeaderInternal
ExtractorTexture::readHeader(BinaryReaderBuffered* HfileReader, BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header)
{
	TextureHeaderExternal extHeader = TextureHeader::readHeader(HfileReader, ensmalleningData);
	TextureHeaderInternal intHeader = TextureHeader::convertHeader(extHeader, FfileReader->getLength(), BfileReader->getLength());
	return intHeader;
}

TextureBodyInternal
ExtractorTexture::readBody(BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header, const TextureHeaderInternal& headerInternal)
{
	BinaryReaderBuffered* bodyReader = FfileReader->getLength() > BfileReader->getLength() ? FfileReader : BfileReader;
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
	bool writeDXT10 = ddspp::encode_header(header.getFormatClass()->format(), header.getWidth(), header.getHeight(), 1, ddspp::Texture2D, 1, 1, *ddsHeader, *ddsHeaderDX10);
	out.write((char*)&magic, 4);
	ddspp::serialize(out, *ddsHeader);
	if (writeDXT10)
		ddspp::serialize(out, *ddsHeaderDX10);

	int32_t mip0Start = body.getDataLen() - header.getMip0Len();
	out.write(body.getData() + mip0Start, header.getMip0Len());
	out.close();
}

void
ExtractorTexture::extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, BinaryReaderBuffered* bReader, BinaryReaderBuffered* fReader, const Ensmallening& ensmalleningData, const std::string& outputPath)
{
	TextureHeaderInternal headerInt = readHeader(hReader, fReader, bReader, ensmalleningData, header);
	TextureBodyInternal body = readBody(fReader, bReader, ensmalleningData, header, headerInt);
	writeData(outputPath, headerInt, body, header);
}

void
ExtractorTexture::extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, BinaryReaderBuffered* bReader, BinaryReaderBuffered* fReader, const Ensmallening& ensmalleningData)
{
	TextureHeaderInternal headerInt = readHeader(hReader, fReader, bReader, ensmalleningData, header);
	TextureBodyInternal body = readBody(fReader, bReader, ensmalleningData, header, headerInt);
}