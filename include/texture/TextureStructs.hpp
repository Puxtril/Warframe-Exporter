#pragma once

#include "TextureInfo.h"

#include <cstdint>
#include <memory>

namespace WarframeExporter::Texture
{
	struct TextureHeaderInternal
	{
		TextureCompression formatEnum;
		TextureInfo* formatClass;
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
		std::unique_ptr<char[]> data;
		size_t dataLen;
	};
}
