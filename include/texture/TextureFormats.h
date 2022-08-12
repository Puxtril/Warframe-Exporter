#pragma once

#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>

#include "TextureEnums.hpp"
#include "ddspp.hpp"

namespace WarframeExporter
{
	namespace Texture
	{
		class TextureFormatBase
		{
		protected:
			TextureFormatBase() = default;
			TextureFormatBase(const TextureFormatBase& other) = delete;
			TextureFormatBase& operator=(const TextureFormatBase& other) = delete;

		public:
			virtual ddspp::DXGIFormat format() = 0;
			virtual void unSwizzle(char* inData, size_t inDataLen, char* outData) = 0;
		};

		TextureFormatBase*
		formatClassFactory(TextureFormatExternal formatEnum);

		namespace Internal
		{
			class TextureFormatBC1 : public TextureFormatBase
			{
			protected:
				TextureFormatBC1() = default;
				TextureFormatBC1(const TextureFormatBC1& other) = delete;
				TextureFormatBC1& operator=(const TextureFormatBC1& other) = delete;

			public:
				static TextureFormatBC1& getInstance();
				ddspp::DXGIFormat format();
				void unSwizzle(char* inData, size_t inDataLen, char* outData);
			};


			class TextureFormatBC2 : public TextureFormatBase
			{
			protected:
				TextureFormatBC2() = default;
				TextureFormatBC2(const TextureFormatBC2& other) = delete;
				TextureFormatBC2& operator=(const TextureFormatBC2& other) = delete;

			public:
				static TextureFormatBC2& getInstance();
				ddspp::DXGIFormat format();
				void unSwizzle(char* inData, size_t inDataLen, char* outData);
			};


			class TextureFormatBC3 : public TextureFormatBase
			{
			protected:
				TextureFormatBC3() = default;
				TextureFormatBC3(const TextureFormatBC3& other) = delete;
				TextureFormatBC3& operator=(const TextureFormatBC3& other) = delete;

			public:
				static TextureFormatBC3& getInstance();
				ddspp::DXGIFormat format();
				void unSwizzle(char* inData, size_t inDataLen, char* outData);
			};


			class TextureFormatBC4 : public TextureFormatBase
			{
			protected:
				TextureFormatBC4() = default;
				TextureFormatBC4(const TextureFormatBC4& other) = delete;
				TextureFormatBC4& operator=(const TextureFormatBC4& other) = delete;

			public:
				static TextureFormatBC4& getInstance();
				ddspp::DXGIFormat format();
				void unSwizzle(char* inData, size_t inDataLen, char* outData);
			};


			class TextureFormatBC5 : public TextureFormatBase
			{
			protected:
				TextureFormatBC5() = default;
				TextureFormatBC5(const TextureFormatBC5& other) = delete;
				TextureFormatBC5& operator=(const TextureFormatBC5& other) = delete;

			public:
				static TextureFormatBC5& getInstance();
				ddspp::DXGIFormat format();
				void unSwizzle(char* inData, size_t inDataLen, char* outData);
			};


			class TextureFormatUncompressed : public TextureFormatBase
			{
			protected:
				TextureFormatUncompressed() = default;
				TextureFormatUncompressed(const TextureFormatUncompressed& other) = delete;
				TextureFormatUncompressed& operator=(const TextureFormatUncompressed& other) = delete;

			public:
				static TextureFormatUncompressed& getInstance();
				ddspp::DXGIFormat format();
				void unSwizzle(char* inData, size_t inDataLen, char* outData);
			};


			class TextureFormatBC6 : public TextureFormatBase
			{
			protected:
				TextureFormatBC6() = default;
				TextureFormatBC6(const TextureFormatBC6& other) = delete;
				TextureFormatBC6& operator=(const TextureFormatBC6& other) = delete;

			public:
				static TextureFormatBC6& getInstance();
				ddspp::DXGIFormat format();
				void unSwizzle(char* inData, size_t inDataLen, char* outData);
			};


			class TextureFormatBC7 : public TextureFormatBase
			{
			protected:
				TextureFormatBC7() = default;
				TextureFormatBC7(const TextureFormatBC7& other) = delete;
				TextureFormatBC7& operator=(const TextureFormatBC7& other) = delete;

			public:
				static TextureFormatBC7& getInstance();
				ddspp::DXGIFormat format();
				void unSwizzle(char* inData, size_t inDataLen, char* outData);
			};
		}
	}
}