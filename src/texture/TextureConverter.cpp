#include "texture/TextureConverter.h"

using namespace WarframeExporter::Texture;

TextureHeaderInternal
TextureConverter::convertHeader(TextureHeaderExternal& headerExternal, int32_t fileSize)
{
	TextureInfo* formatClass = g_enumMapTexture[(int)headerExternal.format];

	bool isCompressed = ddspp::is_compressed(formatClass->getFormat());
	int squareSize = ddspp::get_bits_per_pixel_or_block(formatClass->getFormat()) / 8;
	auto dimensions = getCorrectResolution(headerExternal.widthBase, headerExternal.heightBase, isCompressed, fileSize, squareSize);
	int32_t mip0Len = getMip0Len(std::get<0>(dimensions), std::get<1>(dimensions), isCompressed, squareSize);

	return TextureHeaderInternal{ static_cast<TextureCompression>(headerExternal.format), formatClass, mip0Len, std::get<0>(dimensions), std::get<1>(dimensions) };
}

std::pair<int16_t, int16_t>
TextureConverter::getCorrectResolution(uint16_t baseWidth, uint16_t baseHeight, bool isCompressed, int32_t textureLength, int blockSize)
{
	int32_t curMipSize = getMip0Len(baseWidth, baseHeight, isCompressed, blockSize);
	if (!isCompressed && curMipSize >= textureLength)
		return TextureConverter::getCorrectResolution(baseWidth / 2, baseHeight / 2, isCompressed, textureLength, blockSize);
	if (isCompressed && curMipSize > textureLength)
		return TextureConverter::getCorrectResolution(baseWidth / 2, baseHeight / 2, isCompressed, textureLength, blockSize);
	return std::pair<uint16_t, uint16_t>{baseWidth, baseHeight};
}

int32_t
TextureConverter::getMip0Len(int16_t width, int16_t height, bool isCompressed, int blockSize)
{
	if (!isCompressed)
		return width * height * 4;
	return std::max(1, ((width + 3) / 4)) * std::max(1, ((height + 3) / 4)) * blockSize;
}
