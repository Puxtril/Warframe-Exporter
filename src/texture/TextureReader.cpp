#include "texture/TextureReader.h"

using namespace WarframeExporter::Texture;

BinaryReaderBuffered*
TextureReader::getCorrectBodyReader(BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader)
{
	return FfileReader->getLength() > BfileReader->getLength() ? FfileReader : BfileReader;
}

TextureHeaderExternal
TextureReader::readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData)
{
	uint8_t enum1 = headerReader->readUInt8();
	uint8_t enum2 = headerReader->readUInt8();
	uint8_t enum3 = headerReader->readUInt8();
	uint8_t enum4 = headerReader->readUInt8();

	uint32_t unkArrCount = headerReader->readUInt32();
	headerReader->seek(4U * unkArrCount, std::ios_base::cur);

	if (!ensmalleningData.isPostPart1())
		headerReader->seek(9, std::ios_base::cur);

	uint16_t width = headerReader->readUInt16();
	uint16_t height = headerReader->readUInt16();

	return TextureHeaderExternal{ enum1, enum2, enum3, enum4, width, height };
}

TextureBodyInternal
TextureReader::readBody(BinaryReaderBuffered* bodyReader, const TextureHeaderInternal& headerInternal, const Ensmallening& postEnsmallening)
{
	if (postEnsmallening.isPostPart1())
	{
		std::unique_ptr<char[]> data = std::make_unique<char[]>(bodyReader->getLength());
		std::memcpy(data.get(), bodyReader->getPtr(), bodyReader->getLength());
		return TextureBodyInternal{ std::move(data), bodyReader->getLength() };
	}
	else
	{
		char* rawData = (char*)bodyReader->readUInt8Array(bodyReader->getLength());
		std::unique_ptr<char[]> unSwizzled = std::make_unique<char[]>(bodyReader->getLength());
		headerInternal.formatClass->unSwizzle(rawData, bodyReader->getLength(), unSwizzled.get());
		delete[] rawData;
		return TextureBodyInternal{std::move(unSwizzled), bodyReader->getLength()};
	}
}
