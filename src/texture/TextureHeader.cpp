#include "texture/TextureHeader.h"

using namespace WarframeExporter::Texture;

TextureHeaderExternal
TextureHeader::readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData)
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

TextureHeaderInternal
TextureHeader::convertHeader(TextureHeaderExternal& headerExternal, size_t fileSize)
{
	TextureFormatExternal extFormat = static_cast<TextureFormatExternal>(headerExternal.format);
	TextureFormatBase* formatClass = formatClassFactory(extFormat);

	bool isCompressed = ddspp::is_compressed(formatClass->format());
	int squareSize = ddspp::get_bits_per_pixel_or_block(formatClass->format()) / 8;
	auto dimensions = getCorrectResolution(headerExternal.widthBase, headerExternal.heightBase, isCompressed, fileSize, squareSize);
	uint32_t mip0Len = getMip0Len(std::get<0>(dimensions), std::get<1>(dimensions), isCompressed, squareSize);

	return TextureHeaderInternal{ extFormat, formatClass, mip0Len, std::get<0>(dimensions), std::get<1>(dimensions) };
}

std::pair<uint16_t, uint16_t>
TextureHeader::getCorrectResolution(uint16_t baseWidth, uint16_t baseHeight, bool isCompressed, uint32_t textureLength, int blockSize)
{
	uint32_t curMipSize = getMip0Len(baseWidth, baseHeight, isCompressed, blockSize);
	if (!isCompressed && curMipSize >= textureLength)
		return TextureHeader::getCorrectResolution(baseWidth / 2, baseHeight / 2, isCompressed, textureLength, blockSize);
	if (isCompressed && curMipSize > textureLength)
		return TextureHeader::getCorrectResolution(baseWidth / 2, baseHeight / 2, isCompressed, textureLength, blockSize);
	return std::pair<uint16_t, uint16_t>{baseWidth, baseHeight};
}

uint32_t
TextureHeader::getMip0Len(uint16_t width, uint16_t height, bool isCompressed, int blockSize)
{
	if (!isCompressed)
		return width * height * 4;
	return std::max(1, ((width + 3) / 4)) * std::max(1, ((height + 3) / 4)) * blockSize;
}