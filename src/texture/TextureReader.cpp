#include "texture/TextureReader.h"

using namespace WarframeExporter::Texture;

BinaryReader::BinaryReaderBuffered*
TextureReader::getCorrectBodyReader(BinaryReader::BinaryReaderBuffered* FfileReader, BinaryReader::BinaryReaderBuffered* BfileReader)
{
	return FfileReader->getLength() > BfileReader->getLength() ? FfileReader : BfileReader;
}

TextureHeaderExternal
TextureReader::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
	uint8_t enum1 = headerReader->readUInt8();
	uint8_t enum2 = headerReader->readUInt8();
	uint8_t enum3 = headerReader->readUInt8();
	uint8_t enum4 = headerReader->readUInt8();

	uint32_t unkArrCount = headerReader->readUInt32();
	headerReader->seek(4U * unkArrCount, std::ios_base::cur);

	//if (!ensmalleningData.isPostPart1())
	//	headerReader->seek(9, std::ios_base::cur);

	int16_t width = headerReader->readInt16();
	int16_t height = headerReader->readInt16();

	std::string texPaths = "";

	if (commonHeader.type == (uint32_t)TextureType::TEXTURE_ARRAY)
	{
		headerReader->seek(0x2F, std::ios::cur);

		uint32_t pathLen = headerReader->readUInt32();
		texPaths = headerReader->readAsciiString(pathLen);
	}

	return TextureHeaderExternal{ enum1, enum2, enum3, enum4, width, height, std::move(texPaths) };
}

std::vector<char>
TextureReader::readBody(BinaryReader::BinaryReaderBuffered* bodyReader, const TextureHeaderExternal& headerExternal)
{
	std::vector<char> data(bodyReader->getLength());
	std::memcpy(data.data(), bodyReader->getPtr().data(), bodyReader->getLength());
	return data;
}
