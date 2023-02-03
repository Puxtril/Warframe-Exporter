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
		static TextureHeaderInternal convertHeader(TextureHeaderExternal& headerExternal, size_t fileSize);

	private:
		static std::pair<uint16_t, uint16_t> getCorrectResolution(uint16_t baseWidth, uint16_t baseHeight, bool isCompressed, uint32_t textureLength, int blockSize);
		static uint32_t getMip0Len(uint16_t width, uint16_t height, bool isCompressed, int blockSize);
	};
}