#pragma once

#include "BinaryReaderBuffered.h"
#include "TextureStructs.hpp"
#include "TextureExportTypes.h"

#include <algorithm>

namespace WarframeExporter::Texture
{
	class TextureConverter
	{
	public:
		static TextureHeaderInternal convertHeader(TextureHeaderExternal& headerExternal, int32_t fileSize, uint32_t fileFormat);

	private:
		static std::pair<int16_t, int16_t> getCorrectResolution(uint16_t baseWidth, uint16_t baseHeight, bool isCompressed, int32_t textureLength, int blockSize);
		static int32_t getMip0Len(int16_t width, int16_t height, bool isCompressed, int blockSize);
		static std::vector<std::string> parseSubtextureString(const std::string& subtextureString);
	};
}
