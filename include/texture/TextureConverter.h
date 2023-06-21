#pragma once

#include "BinaryReaderBuffered.h"
#include "../Ensmallening.hpp"
#include "TextureStructs.hpp"
#include "TextureEnumMap.h"

#include <algorithm>

namespace WarframeExporter::Texture
{
	class TextureConverter
	{
	public:
		static TextureHeaderInternal convertHeader(TextureHeaderExternal& headerExternal, int32_t fileSize);

	private:
		static std::pair<int16_t, int16_t> getCorrectResolution(uint16_t baseWidth, uint16_t baseHeight, bool isCompressed, int32_t textureLength, int blockSize);
		static int32_t getMip0Len(int16_t width, int16_t height, bool isCompressed, int blockSize);
	};
}
