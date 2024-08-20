#include "texture/TextureExporterConvert.h"
#include "spng.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define BCDEC_IMPLEMENTATION

#include "bcdec.h"
#include "stb_image_write.h"

using namespace WarframeExporter::Texture;

void
TextureExporterConvert::convertAndWriteToPng(const char* data, size_t dataLen, const std::filesystem::path& outPath, TextureCompression compression, uint32_t width, uint32_t height)
{  
    spng_ctx* pngEncodeContext = spng_ctx_new(SPNG_CTX_ENCODER);

    FILE* outFile = fopen(outPath.string().c_str(), "w+b");
    spng_set_png_file(pngEncodeContext, outFile);
    
    int ret;
    if (compression == TextureCompression::Uncompressed)
    {
        spng_ihdr ihdr = { width, height, 8, SPNG_COLOR_TYPE_TRUECOLOR_ALPHA };
        spng_set_ihdr(pngEncodeContext, &ihdr);
        ret = spng_encode_image(pngEncodeContext, data, dataLen, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
    }
    else if (compression == TextureCompression::A8)
    {
        spng_ihdr ihdr = { width, height, 8, SPNG_COLOR_TYPE_GRAYSCALE };
        spng_set_ihdr(pngEncodeContext, &ihdr);
        ret = spng_encode_image(pngEncodeContext, data, dataLen, SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
    }
    else
    {
        BCConvertInfo info = getConvertInfo(compression);
        std::vector<char> decompressed(width * height * info.blockSizeDecompressed);
        decompressDdsBlocks(data, dataLen, decompressed, width, height, info.blockSizeSource, info.blockSizeDecompressed, info.decodeFunction);
        
        spng_ihdr ihdr = { width, height, 8 , info.pngColorType };
        spng_set_ihdr(pngEncodeContext, &ihdr);
        ret = spng_encode_image(pngEncodeContext, decompressed.data(), decompressed.size(), SPNG_FMT_PNG, SPNG_ENCODE_FINALIZE);
    }

    if(ret)
        WarframeExporter::Logger::getInstance().error("spng_encode_image() error: " + std::string(spng_strerror(ret)));

    fclose(outFile);
    spng_ctx_free(pngEncodeContext);
}

void
TextureExporterConvert::convertAndWriteToHdr(const char* data, size_t dataLen, const std::filesystem::path& outPath, uint32_t width, uint32_t height)
{ 
    BCConvertInfo info = getConvertInfo(TextureCompression::BC6);

    std::vector<char> decompressed(width * height * info.blockSizeSource);
    decompressDdsBlocks(data, dataLen, decompressed, width, height, info.blockSizeDecompressed, info.blockSizeSource, &TextureExporterConvert::bc6h_float);

    stbi_write_hdr(outPath.string().c_str(), width, height, 3, (float*)decompressed.data());
}

void
TextureExporterConvert::convertAndWriteToTga(const char* data, size_t dataLen, const std::filesystem::path& outPath, TextureCompression compression, uint32_t width, uint32_t height)
{
    if (compression == TextureCompression::Uncompressed)
    {
        stbi_write_tga(outPath.string().c_str(), width, height, 4, data);
    }
    else if (compression == TextureCompression::A8)
    {
        std::vector<char> aToRgbaBuffer(dataLen * 4);
        for (size_t i = 0; i < dataLen; i++)
            aToRgbaBuffer[i * 3] = aToRgbaBuffer[i * 3 + 1] = aToRgbaBuffer[i * 3 + 2] = data[i];
        stbi_write_tga(outPath.string().c_str(), width, height, 3, aToRgbaBuffer.data());
    }
    else
    {
        BCConvertInfo info = getConvertInfo(compression);
        std::vector<char> decompressed(width * height * info.blockSizeSource);
        decompressDdsBlocks(data, dataLen, decompressed, width, height, info.blockSizeSource, info.blockSizeDecompressed, info.decodeFunction);

        stbi_write_tga(outPath.string().c_str(), width, height, info.blockSizeDecompressed, decompressed.data());
    }
}

void
TextureExporterConvert::decompressDdsBlocks(const char* data, size_t dataLen, std::vector<char>& decompressed, int width, int height, int srcBlockSize, int dstBlockSize, decodeFunc decodeFunc)
{
    const char* src = data;
    char* dst;

    for (int i = 0; i < height; i += 4)
    {
        for (int j = 0; j < width; j += 4)
        {
            dst = decompressed.data() + (i * width + j) * dstBlockSize;
            decodeFunc(src, dst, width * dstBlockSize);
            src += srcBlockSize;
        }
    }
}

void
TextureExporterConvert::bc6h_float(const void* compressedBlock, void* decompressedBlock, int destinationPitch)
{
    bcdec_bc6h_float(compressedBlock, decompressedBlock, destinationPitch, false);
}

void
TextureExporterConvert::bcdec_bc5_as_RGB(const void* compressedBlock, void* decompressedBlock, int destinationPitch)
{
    bcdec__smooth_alpha_block(compressedBlock, decompressedBlock, destinationPitch, 3);
    bcdec__smooth_alpha_block(((char*)compressedBlock) + 8, ((char*)decompressedBlock) + 1, destinationPitch, 3);
}

TextureExporterConvert::BCConvertInfo
TextureExporterConvert::getConvertInfo(TextureCompression compression)
{
    switch(compression)
    {
        case TextureCompression::Default:
        case TextureCompression::BC1:
            return TextureExporterConvert::BC1ConvertInfo;
        case TextureCompression::BC2:
            return TextureExporterConvert::BC2ConvertInfo;
        case TextureCompression::BC3:
            return TextureExporterConvert::BC3ConvertInfo;
        case TextureCompression::BC4:
            return TextureExporterConvert::BC4ConvertInfo;
        case TextureCompression::BC5:
            return TextureExporterConvert::BC5ConvertInfo;
        case TextureCompression::BC6:
            return TextureExporterConvert::BC6ConvertInfo;
        case TextureCompression::BC7:
            return TextureExporterConvert::BC7ConvertInfo;
        case TextureCompression::Uncompressed:
            throw std::runtime_error("No conversion info for uncompressed format");
        default:
            throw std::runtime_error("Compression format not implemented for conversion");
    }
}