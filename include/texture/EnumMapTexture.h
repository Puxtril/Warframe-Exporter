#pragma once

#include "EnumMap.h"
#include "TextureFormats.h"

namespace WarframeExporter::Texture
{
	const static EnumMap<TextureInfo> g_enumMapTexture = EnumMap<TextureInfo>()
		.registerClass(TextureInfoBC1::getInstance())
		.registerClass(TextureInfoBC2::getInstance())
		.registerClass(TextureInfoBC3::getInstance())
		.registerClass(TextureInfoBC4::getInstance())
		.registerClass(TextureInfoBC5::getInstance())
		.registerClass(TextureInfoBC6::getInstance())
		.registerClass(TextureInfoBC7::getInstance())
		.registerClass(TextureInfoUncompressed::getInstance());
}