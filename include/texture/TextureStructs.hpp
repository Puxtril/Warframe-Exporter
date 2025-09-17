#pragma once

#include "TextureTypes.h"
#include "ddspp.h"

#include <string>
#include <cstdint>
#include <vector>

namespace WarframeExporter::Texture
{
	struct TextureHeaderExternal
	{
		uint8_t unkEnum1;
		uint8_t unkEnum2;
		uint8_t unkEnum3;
		uint8_t format;
		int16_t widthBase;
		int16_t heightBase;
		std::string textureNames;
	};

	struct TextureHeaderInternal
	{
		TextureCompression formatEnum;
		ddspp::DXGIFormat ddsFormat;
		int32_t mip0Len;
		int16_t width;
		int16_t height;
		std::vector<std::string> textureNames;
	};

	struct TextureInternal
	{
		TextureHeaderInternal header;
		std::vector<char> body;
	};
}
