#pragma once

#include <map>

#include "ddspp.h"
#include "texture/TextureTypes.h"

namespace WarframeExporter::Texture
{
    const static std::map<TextureCompression, ddspp::DXGIFormat> internalFormatToDdsFormat {
		{TextureCompression::Default, ddspp::DXGIFormat::BC1_UNORM},
		{TextureCompression::BC1, ddspp::DXGIFormat::BC1_UNORM},
		{TextureCompression::BC2, ddspp::DXGIFormat::BC2_UNORM},
		{TextureCompression::BC3, ddspp::DXGIFormat::BC3_UNORM},
		{TextureCompression::BC4, ddspp::DXGIFormat::BC4_UNORM},
		{TextureCompression::BC5, ddspp::DXGIFormat::BC5_UNORM},
		{TextureCompression::BC6, ddspp::DXGIFormat::BC6H_UF16},
		{TextureCompression::BC7, ddspp::DXGIFormat::BC7_UNORM},
		{TextureCompression::Uncompressed, ddspp::DXGIFormat::B8G8R8A8_UNORM},
		{TextureCompression::A8, ddspp::DXGIFormat::A8_UNORM},
		{TextureCompression::R16, ddspp::DXGIFormat::R16_UNORM}
	};

    enum TextureExportType
    {
        TEXTURE_EXPORT_DDS = 1,
        TEXTURE_EXPORT_PNG = 2,
        TEXTURE_EXPORT_TGA = 3
    };
};