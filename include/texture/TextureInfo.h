#pragma once

#include "EnumMapValue.h"
#include "ddspp.h"
#include "texture/TextureTypes.h"

namespace WarframeExporter::Texture
{
	class TextureInfo : public EnumMapValue
	{
	protected:
		TextureInfo() = default;

	public:
		virtual ddspp::DXGIFormat getFormat() const = 0;
		virtual void unSwizzle(char* inData, int inDataLen, char* outData) const = 0;
	};
}