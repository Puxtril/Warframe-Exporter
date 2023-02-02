#pragma once

#include "EnumMapValue.h"
#include "texture/ddspp.hpp"

namespace WarframeExporter::Texture
{
	enum class TextureCompression
	{
		Default = 0,
		BC1 = 1,
		BC2 = 2,
		BC3 = 3,
		BC4 = 6,
		BC5 = 7,
		Uncompressed = 10,
		BC7 = 34,
		BC6 = 35,
	};

	class TextureInfo : public EnumMapValue
	{
	protected:
		TextureInfo() = default;

	public:
		virtual ddspp::DXGIFormat getFormat() const = 0;
		virtual void unSwizzle(char* inData, size_t inDataLen, char* outData) const = 0;
	};
}