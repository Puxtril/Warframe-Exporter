#pragma once

#include "TextureFormats.h"

#include <cstdint>

namespace WarframeExporter
{
	namespace Texture
	{
		struct TextureHeaderInternal
		{
			TextureFormatExternal formatEnum;
			TextureFormatBase* formatClass;
			uint32_t mip0Len;
			uint16_t width;
			uint16_t height;
		};

		struct TextureHeaderExternal
		{
			uint8_t unkEnum1;
			uint8_t unkEnum2;
			uint8_t unkEnum3;
			uint8_t format;
			uint16_t widthBase;
			uint16_t heightBase;
		};

		struct TextureBodyInternal
		{
			const char* data;
			size_t dataLen;
			bool sharedData;

			~TextureBodyInternal()
			{
				if (!sharedData)
					delete[] data;
			}
		};
	}
}