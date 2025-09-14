#pragma once

namespace WarframeExporter::Texture
{
	// Types used in the Texture package
    enum class TextureType
    {
		TEXTURE_DIFFUSE_EMISSION_TINT = 163,
		TEXTURE_BILLBOARD_SPRITEMAP_DIFFUSE = 164,
		TEXTURE_BILLBOARD_SPRITEMAP_NORMAL = 165,
		TEXTURE_ARRAY = 166,
		TEXTURE_ROUGHNESS = 167,
		TEXTURE_SKYBOX = 171,
		TEXTURE_174 = 174,
		TEXTURE_RAW_175 = 175,
		TEXTURE_176 = 176,
		TEXTURE_CUBEMAP = 177,
		TEXTURE_NORMAL_MAP = 184,
		TEXTURE_PACKMAP = 188,
		TEXTURE_194 = 194,
		TEXTURE_DETAILSPACK = 195
	};

	// Types used in the Lightmap package
	enum class LightmapType
    {
		LIGHTMAP_163 = 163,
		LIGHTMAP_164 = 164,
		LIGHTMAP_166 = 166,
		LIGHTMAP_175 = 175,
		LIGHTMAP_176 = 176,
		LIGHTMAP_177 = 177,
		LIGHTMAP_HDR = 181,
	};

    enum class TextureCompression
	{
		Default = 0,
		BC1 = 1,
		BC2 = 2,
		BC3 = 3,
		BC4 = 6,
		BC5 = 7,
		Uncompressed = 10,
		A8 = 12,
		R16 = 30,
		BC7 = 34,
		BC6 = 35,
	};
}