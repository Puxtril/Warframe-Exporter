#pragma once

#include "BinaryReaderBuffered.h"
#include "../Ensmallening.hpp"
#include "TextureStructs.hpp"
#include "TextureFormats.h"

#include <algorithm>

namespace WarframeExporter
{
	namespace Texture
	{
		class TextureHeader
		{
		public:
			static TextureHeaderExternal readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData);
			static TextureHeaderInternal convertHeader(TextureHeaderExternal& headerExternal, size_t fileSize);

		private:
			static std::pair<uint16_t, uint16_t> getCorrectResolution(uint16_t baseWidth, uint16_t baseHeight, bool isCompressed, uint32_t textureLength, int blockSize);
			static uint32_t getMip0Len(uint16_t width, uint16_t height, bool isCompressed, int blockSize);
		};
	}
}