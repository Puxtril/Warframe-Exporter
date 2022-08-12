#pragma once

#include "TextureFormats.h"

#include <cstdint>

namespace WarframeExporter
{
	namespace Texture
	{
		struct TextureHeaderInternal
		{
		private:
			TextureFormatExternal formatEnum;
			TextureFormatBase* formatClass;
			uint32_t mip0Len;
			uint16_t width;
			uint16_t height;

		public:
			TextureHeaderInternal(TextureFormatExternal formatEnum, TextureFormatBase* formatClass, uint32_t mip0Len, uint16_t width, uint16_t height)
				: formatEnum(formatEnum), formatClass(formatClass), mip0Len(mip0Len), width(width), height(height)
			{}

			TextureFormatExternal getFormatEnum() const { return formatEnum; }
			TextureFormatBase* getFormatClass() const { return formatClass; }
			uint32_t getMip0Len() const { return mip0Len; }
			uint16_t getWidth() const { return width; }
			uint16_t getHeight() const { return height; }
		};

		struct TextureHeaderExternal
		{
		private:
			uint8_t unkEnum1;
			uint8_t unkEnum2;
			uint8_t unkEnum3;
			uint8_t format;
			uint16_t widthBase;
			uint16_t heightBase;

		public:
			TextureHeaderExternal(uint8_t enum1, uint8_t enum2, uint8_t enum3, uint8_t enum4, uint16_t widthBase, uint16_t heightBase)
				: unkEnum1(enum1), unkEnum2(enum2), unkEnum3(enum3), format(enum4), widthBase(widthBase), heightBase(heightBase)
			{}

			uint8_t getEnum1() { return unkEnum1; }
			uint8_t getEnum2() { return unkEnum1; }
			uint8_t getEnum3() { return unkEnum1; }
			uint8_t getFormat() { return format; }
			uint16_t getWidthBase() { return widthBase; }
			uint16_t getHeightBase() { return heightBase; }
		};

		struct TextureBodyInternal
		{
		private:
			const char* data;
			size_t dataLen;
			bool sharedData;

			TextureBodyInternal(const TextureBodyInternal& other) = delete;
			TextureBodyInternal& operator=(const TextureBodyInternal& other) = delete;

		public:
			TextureBodyInternal(const char* data, size_t dataLen, bool sharedData)
				: data(data), dataLen(dataLen), sharedData(sharedData)
			{}
			~TextureBodyInternal()
			{
				if (!sharedData)
					delete[] data;
			}

			const char* getData() const { return data; }
			size_t getDataLen() const { return dataLen; }
		};
	}
}