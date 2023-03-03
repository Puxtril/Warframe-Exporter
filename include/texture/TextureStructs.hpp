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
		int32_t mip0Len;
		int16_t width;
		int16_t height;
	};

	struct TextureHeaderExternal
	{
		uint8_t unkEnum1;
		uint8_t unkEnum2;
		uint8_t unkEnum3;
		uint8_t format;
		int16_t widthBase;
		int16_t heightBase;
	};

	struct TextureBodyInternal
	{
		std::unique_ptr<char[]> data;
		size_t dataLen;
	};
}
