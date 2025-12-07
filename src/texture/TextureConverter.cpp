#include "texture/TextureConverter.h"

using namespace WarframeExporter::Texture;

TextureHeaderInternal
TextureConverter::convertHeader(TextureHeaderExternal& headerExternal, int32_t fileSize, uint32_t fileFormat)
{
	TextureCompression compressionFormat = static_cast<TextureCompression>(headerExternal.format);
	ddspp::DXGIFormat ddsFormat = internalFormatToDdsFormat.at(compressionFormat);

	std::vector<std::string> subtextureNames;
	if (fileFormat == (uint32_t)TextureType::TEXTURE_CUBEMAP)
	{
		// Cubemaps will have exactly 6 sub-textures
		subtextureNames = {"1", "2", "3", "4", "5", "6"};
	}
	else
	{
		// Textures of type TEXTURE_ARRAY will have X sub-textures
		// But that is read by the external reader. This will just parse this field
		subtextureNames = parseSubtextureString(headerExternal.textureNames);
	}
	size_t filesizeMinusTextureCount = subtextureNames.size() > 0 ? fileSize / subtextureNames.size() : fileSize;

	bool isCompressed = ddspp::is_compressed(ddsFormat);
	int squareSize = ddspp::get_bits_per_pixel_or_block(ddsFormat) / 8;
	int32_t singleEntryFileSize = std::ceil(filesizeMinusTextureCount);
	auto dimensions = getCorrectResolution(headerExternal.widthBase, headerExternal.heightBase, isCompressed, singleEntryFileSize, squareSize);
	int32_t mip0Len = getMip0Len(std::get<0>(dimensions), std::get<1>(dimensions), isCompressed, squareSize);

	return TextureHeaderInternal{ static_cast<TextureCompression>(headerExternal.format), ddsFormat, mip0Len, std::get<0>(dimensions), std::get<1>(dimensions), subtextureNames };
}

void
TextureConverter::flipTextureChannels(char* data, size_t dataLen)
{
	for (size_t i = 0; i < dataLen; i += 4)
		std::swap(data[i], data[i+2]);
}

std::pair<int16_t, int16_t>
TextureConverter::getCorrectResolution(uint16_t baseWidth, uint16_t baseHeight, bool isCompressed, int32_t textureLength, int blockSize)
{
	int32_t curMipSize = getMip0Len(baseWidth, baseHeight, isCompressed, blockSize);
	if (!isCompressed && curMipSize > textureLength)
		return TextureConverter::getCorrectResolution(baseWidth / 2, baseHeight / 2, isCompressed, textureLength, blockSize);
	if (isCompressed && curMipSize > textureLength)
		return TextureConverter::getCorrectResolution(baseWidth / 2, baseHeight / 2, isCompressed, textureLength, blockSize);
	return std::pair<uint16_t, uint16_t>{baseWidth, baseHeight};
}

int32_t
TextureConverter::getMip0Len(int16_t width, int16_t height, bool isCompressed, int blockSize)
{
	if (!isCompressed)
		return width * height * blockSize;
	return std::max(1, ((width + 3) / 4)) * std::max(1, ((height + 3) / 4)) * blockSize;
}

std::vector<std::string>
TextureConverter::parseSubtextureString(const std::string& subtextureString)
{
	std::vector<std::string> subtexturePaths;

	size_t cursor1 = 0;
	size_t cursor2 = subtextureString.find('.', cursor1);

	while(cursor2 < subtextureString.length())
	{
		size_t startOfNext = subtextureString.find('/', cursor2 + 1);
		subtexturePaths.push_back(subtextureString.substr(cursor1, startOfNext - cursor1));

		cursor1 = startOfNext;
		cursor2 = subtextureString.find('.', cursor1);
	}

	return subtexturePaths;
}
