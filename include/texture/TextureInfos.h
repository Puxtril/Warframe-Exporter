#pragma once

#include <cstring>
#include <algorithm>
#include <string>
#include <iostream>

#include "TextureInfo.h"

namespace WarframeExporter::Texture
{
	///////////////////////////////////////////////////////////////
	// BC1

	class TextureInfoBC1 : public TextureInfo
	{
	protected:
		TextureInfoBC1() = default;

	public:
		TextureInfoBC1(const TextureInfoBC1& other) = delete;
		TextureInfoBC1& operator=(const TextureInfoBC1& other) = delete;

		static TextureInfoBC1* getInstance();

		std::vector<int> getEnumMapKeys() const override;
		ddspp::DXGIFormat getFormat() const override;
		void unSwizzle(char* inData, int inDataLen, char* outData) const override;
	};

	///////////////////////////////////////////////////////////////
	// BC2

	class TextureInfoBC2 : public TextureInfo
	{
	protected:
		TextureInfoBC2() = default;

	public:
		TextureInfoBC2(const TextureInfoBC2& other) = delete;
		TextureInfoBC2& operator=(const TextureInfoBC2& other) = delete;

		static TextureInfoBC2* getInstance();

		std::vector<int> getEnumMapKeys() const override;
		ddspp::DXGIFormat getFormat() const override;
		void unSwizzle(char* inData, int inDataLen, char* outData) const override;
	};

	///////////////////////////////////////////////////////////////
	// BC3

	class TextureInfoBC3 : public TextureInfo
	{
	protected:
		TextureInfoBC3() = default;

	public:
		TextureInfoBC3(const TextureInfoBC3& other) = delete;
		TextureInfoBC3& operator=(const TextureInfoBC3& other) = delete;

		static TextureInfoBC3* getInstance();
		
		std::vector<int> getEnumMapKeys() const override;
		ddspp::DXGIFormat getFormat() const override;
		void unSwizzle(char* inData, int inDataLen, char* outData) const override;
	};

	///////////////////////////////////////////////////////////////
	// BC4

	class TextureInfoBC4 : public TextureInfo
	{
	protected:
		TextureInfoBC4() = default;

	public:
		TextureInfoBC4(const TextureInfoBC4& other) = delete;
		TextureInfoBC4& operator=(const TextureInfoBC4& other) = delete;
		
		static TextureInfoBC4* getInstance();
		
		std::vector<int> getEnumMapKeys() const override;
		ddspp::DXGIFormat getFormat() const override;
		void unSwizzle(char* inData, int inDataLen, char* outData) const override;
	};

	///////////////////////////////////////////////////////////////
	// BC5

	class TextureInfoBC5 : public TextureInfo
	{
	protected:
		TextureInfoBC5() = default;

	public:
		TextureInfoBC5(const TextureInfoBC5& other) = delete;
		TextureInfoBC5& operator=(const TextureInfoBC5& other) = delete;

		static TextureInfoBC5* getInstance();
		
		std::vector<int> getEnumMapKeys() const override;
		ddspp::DXGIFormat getFormat() const override;
		void unSwizzle(char* inData, int inDataLen, char* outData) const override;
	};

	///////////////////////////////////////////////////////////////
	// Uncompressed

	class TextureInfoUncompressed : public TextureInfo
	{
	protected:
		TextureInfoUncompressed() = default;

	public:
		TextureInfoUncompressed(const TextureInfoUncompressed& other) = delete;
		TextureInfoUncompressed& operator=(const TextureInfoUncompressed& other) = delete;
		
		static TextureInfoUncompressed* getInstance();

		std::vector<int> getEnumMapKeys() const override;
		ddspp::DXGIFormat getFormat() const override;
		void unSwizzle(char* inData, int inDataLen, char* outData) const override;
	};

	///////////////////////////////////////////////////////////////
	// BC6

	class TextureInfoBC6 : public TextureInfo
	{
	protected:
		TextureInfoBC6() = default;

	public:
		TextureInfoBC6(const TextureInfoBC6& other) = delete;
		TextureInfoBC6& operator=(const TextureInfoBC6& other) = delete;

		static TextureInfoBC6* getInstance();

		std::vector<int> getEnumMapKeys() const override;
		ddspp::DXGIFormat getFormat() const override;
		// Format was introduced after swizzling was removed. So this should never be called.
		void unSwizzle(char* inData, int inDataLen, char* outData) const override;
	};

	///////////////////////////////////////////////////////////////
	// BC7

	class TextureInfoBC7 : public TextureInfo
	{
	protected:
		TextureInfoBC7() = default;

	public:
		TextureInfoBC7(const TextureInfoBC7& other) = delete;
		TextureInfoBC7& operator=(const TextureInfoBC7& other) = delete;

		static TextureInfoBC7* getInstance();

		std::vector<int> getEnumMapKeys() const override;
		ddspp::DXGIFormat getFormat() const override;
		// Format was introduced after swizzling was removed. So this should never be called.
		void unSwizzle(char* inData, int inDataLen, char* outData) const override;
	};
}