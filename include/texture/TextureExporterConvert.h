#pragma once

// stb_image_write.h: 'sprintf'
// Also `fopen` in TextureExporterConvert.cpp
#define _CRT_SECURE_NO_WARNINGS

#include "ExporterLogger.h"
#include "bcdec.h"
#include "spng.h"
#include "stb_image_write.h"
#include "texture/TextureTypes.h"

#include <cstdlib>
#include <stdexcept>
#include <string>
#include <filesystem>
#include <fstream>
#include <vector>
#include <iostream>
#include <cstring>

// Forward declaration
// Implementation in spng library
//static void bcdec__smooth_alpha_block(const void* compressedBlock, void* decompressedBlock, int destinationPitch, int pixelSize);

namespace WarframeExporter::Texture
{

    class TextureExporterConvert
    {
    protected:
        // Generalize the BC decode functions
        typedef void (*decodeFunc)(const void*, void*, int);

        struct BCConvertInfo
        {
            uint8_t pngColorType;
            int blockSizeSource;
            int blockSizeDecompressed;
            decodeFunc decodeFunction;
        };

    public:
        // These can be used with everything EXCEPT BC6
        static void convertAndWriteToPng(const char* data, size_t dataLen, const std::filesystem::path& outPath, TextureCompression compression, uint32_t width, uint32_t height);
        static void convertAndWriteToTga(const char* data, size_t dataLen, const std::filesystem::path& outPath, TextureCompression compression, uint32_t width, uint32_t height);

        // This can only be used with BC6
        static void convertAndWriteToHdr(const char* data, size_t dataLen, const std::filesystem::path& outPath, uint32_t width, uint32_t height);

    protected:
        static void decompressDdsBlocks(const char* data, size_t dataLen, std::vector<char>& decompressed, int width, int height, int srcBlockSize, int dstBlockSize, decodeFunc decodeFunc);

        static void bc6h_float(const void* compressedBlock, void* decompressedBlock, int destinationPitch);

        // Png doesn't have a format for just RG channels
        // Decompress BC5 but leave blank Blue pixel data
        static void bcdec_bc5_as_RGB(const void* compressedBlock, void* decompressedBlock, int destinationPitch);

        static BCConvertInfo getConvertInfo(TextureCompression compression);

        static constexpr BCConvertInfo BC1ConvertInfo {SPNG_COLOR_TYPE_TRUECOLOR_ALPHA, BCDEC_BC1_BLOCK_SIZE, 4, &bcdec_bc1};
        static constexpr BCConvertInfo BC2ConvertInfo {SPNG_COLOR_TYPE_TRUECOLOR_ALPHA, BCDEC_BC2_BLOCK_SIZE, 4, &bcdec_bc2};
        static constexpr BCConvertInfo BC3ConvertInfo {SPNG_COLOR_TYPE_TRUECOLOR_ALPHA, BCDEC_BC3_BLOCK_SIZE, 4, &bcdec_bc3};
        static constexpr BCConvertInfo BC4ConvertInfo {SPNG_COLOR_TYPE_GRAYSCALE, BCDEC_BC4_BLOCK_SIZE, 1, &bcdec_bc4};
        static constexpr BCConvertInfo BC5ConvertInfo {SPNG_COLOR_TYPE_TRUECOLOR, BCDEC_BC5_BLOCK_SIZE, 3, &TextureExporterConvert::bcdec_bc5_as_RGB};
        static constexpr BCConvertInfo BC6ConvertInfo {0, BCDEC_BC6H_BLOCK_SIZE, 3, &TextureExporterConvert::bc6h_float};
        static constexpr BCConvertInfo BC7ConvertInfo {SPNG_COLOR_TYPE_TRUECOLOR_ALPHA, BCDEC_BC7_BLOCK_SIZE, 4, &bcdec_bc7};
    };
};