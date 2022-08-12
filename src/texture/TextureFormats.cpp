#include "texture/TextureFormats.h"

using namespace WarframeExporter;

Texture::TextureFormatBase*
Texture::formatClassFactory(Texture::TextureFormatExternal formatEnum)
{
    switch (formatEnum)
    {
    case TextureFormatExternal::BC1:
        return &Texture::Internal::TextureFormatBC1::getInstance();
    case TextureFormatExternal::BC2:
        return &Texture::Internal::TextureFormatBC2::getInstance();
    case TextureFormatExternal::BC3:
        return &Texture::Internal::TextureFormatBC3::getInstance();
    case TextureFormatExternal::BC4:
        return &Texture::Internal::TextureFormatBC4::getInstance();
    case TextureFormatExternal::BC5:
        return &Texture::Internal::TextureFormatBC5::getInstance();
    case TextureFormatExternal::BC6:
        return &Texture::Internal::TextureFormatBC6::getInstance();
    case TextureFormatExternal::BC7:
        return &Texture::Internal::TextureFormatBC7::getInstance();
    case TextureFormatExternal::Uncompressed:
        return &Texture::Internal::TextureFormatUncompressed::getInstance();
    case TextureFormatExternal::Default:
    default:
        return &Texture::Internal::TextureFormatBC1::getInstance();
    }
}

Texture::Internal::TextureFormatBC1&
Texture::Internal::TextureFormatBC1::getInstance()
{
    static Texture::Internal::TextureFormatBC1 instance;
    return instance;
}

ddspp::DXGIFormat
Texture::Internal::TextureFormatBC1::format()
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC1_UNORM;
    return format;
}

void
Texture::Internal::TextureFormatBC1::unSwizzle(char* inData, size_t inDataLen, char* outData)
{
    const static int WFBLOCKSZ = 0x2000;
    const static int WriteBlockSz = 0x0004;

    char pixels0[0x2000];
    char pixels1[0x2000];
    int inPos = 0;
    int outPos = 0;

    //for (int i = 0; i < std::max(inDataLen / (2 * WFBLOCKSZ), 1ULL); i++)
    for (size_t i = 0; i < inDataLen / (2 * WFBLOCKSZ); i++)
    {
        memcpy(pixels0, inData + inPos, WFBLOCKSZ);
        inPos += WFBLOCKSZ;
        memcpy(pixels1, inData + inPos, WFBLOCKSZ);
        inPos += WFBLOCKSZ;

        int curBlockLen = std::min((size_t)(WFBLOCKSZ / WriteBlockSz), inDataLen - inPos);
        for (int j = 0; j < curBlockLen; j++)
        {
            int startPos1 = j * WriteBlockSz;
            memcpy(outData + outPos, pixels0 + startPos1, WriteBlockSz);
            outPos += WriteBlockSz;
            memcpy(outData + outPos, pixels1 + startPos1, WriteBlockSz);
            outPos += WriteBlockSz;
        }
    }
}

Texture::Internal::TextureFormatBC2&
Texture::Internal::TextureFormatBC2::getInstance()
{
    static Texture::Internal::TextureFormatBC2 instance;
    return instance;
}

ddspp::DXGIFormat
Texture::Internal::TextureFormatBC2::format()
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC2_UNORM;
    return format;
}

void
Texture::Internal::TextureFormatBC2::unSwizzle(char* inData, size_t inDatLen, char* outData)
{
    char* buffer = new char[inDatLen];
    Texture::Internal::TextureFormatBC1::getInstance().unSwizzle(inData, inDatLen, buffer);
    Texture::Internal::TextureFormatBC1::getInstance().unSwizzle(buffer, inDatLen, outData);
    delete[] buffer;
}

Texture::Internal::TextureFormatBC3&
Texture::Internal::TextureFormatBC3::getInstance()
{
    static Texture::Internal::TextureFormatBC3 instance;
    return instance;
}

ddspp::DXGIFormat
Texture::Internal::TextureFormatBC3::format()
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC3_UNORM;
    return format;
}

void
Texture::Internal::TextureFormatBC3::unSwizzle(char* inData, size_t inDatLen, char* outData)
{
    char* buffer = new char[inDatLen];
    Texture::Internal::TextureFormatBC1::getInstance().unSwizzle(inData, inDatLen, buffer);
    Texture::Internal::TextureFormatBC1::getInstance().unSwizzle(buffer, inDatLen, outData);
    delete[] buffer;
}

Texture::Internal::TextureFormatBC4&
Texture::Internal::TextureFormatBC4::getInstance()
{
    static Texture::Internal::TextureFormatBC4 instance;
    return instance;
}

ddspp::DXGIFormat
Texture::Internal::TextureFormatBC4::format()
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC4_UNORM;
    return format;
}

void
Texture::Internal::TextureFormatBC4::unSwizzle(char* inData, size_t inDatLen, char* outData)
{
    const static int WFBlockSize = 0x4000;
    int channelBlockCount = inDatLen / WFBlockSize;
    const static int block0Size = WFBlockSize / 4;
    const static int block1Size = WFBlockSize / 4 * 3;

    static char redBlock0[block0Size];
    static char redBlock1[block1Size];

    for (int i = 0; i < channelBlockCount; i++)
    {
        memcpy(inData + i * WFBlockSize, redBlock0, block0Size);
        memcpy(inData + (i * WFBlockSize + block0Size), redBlock1, block1Size);

        for (int j = 0; j < (block0Size) / 2; j++)
        {
            int baseDestPos = (i * WFBlockSize) + (j * 8);
            memcpy(redBlock0 + j, outData + baseDestPos, 1);
            memcpy(redBlock0 + j + (block0Size / 2), outData + baseDestPos + 1, 1);
            memcpy(redBlock1 + j * 6, outData + baseDestPos + 2, 6);
        }
    }
}

Texture::Internal::TextureFormatBC5&
Texture::Internal::TextureFormatBC5::getInstance()
{
    static Texture::Internal::TextureFormatBC5 instance;
    return instance;
}

ddspp::DXGIFormat
Texture::Internal::TextureFormatBC5::format()
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC5_UNORM;
    return format;
}

void
Texture::Internal::TextureFormatBC5::unSwizzle(char* inData, size_t inDatLen, char* outData)
{
    const static int WFChannelBlockSize = 0x2000;
    const static int block0Size = WFChannelBlockSize / 4;
    const static int block1Size = WFChannelBlockSize / 4 * 3;

    static char redBlock0[block0Size];
    static char redBlock1[block1Size];
    static char greenBlock0[block0Size];
    static char greenBlock1[block1Size];

    for (size_t i = 0; i < inDatLen / WFChannelBlockSize; i += 2)
    {
        memcpy(inData + i * WFChannelBlockSize, redBlock0, block0Size);
        memcpy(inData + i * WFChannelBlockSize + block0Size, redBlock1, block1Size);
        memcpy(inData + ((i + 1) * WFChannelBlockSize), greenBlock0, block0Size);
        memcpy(inData + ((i + 1) * WFChannelBlockSize + block0Size), greenBlock1, block1Size);

        for (int j = 0; j < block0Size; j += 2)
        {
            int baseDestPos = (i * WFChannelBlockSize) + (j * 8);
            memcpy(redBlock0 + j, outData + baseDestPos, 2);
            memcpy(redBlock1 + j * 3, outData + baseDestPos + 2, 6);
            memcpy(greenBlock0 + j, outData + baseDestPos + 8, 2);
            memcpy(greenBlock1 + j * 3, outData + baseDestPos + 10, 6);
        }
    }
}

Texture::Internal::TextureFormatUncompressed&
Texture::Internal::TextureFormatUncompressed::getInstance()
{
    static Texture::Internal::TextureFormatUncompressed instance;
    return instance;
}

ddspp::DXGIFormat
Texture::Internal::TextureFormatUncompressed::format()
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::R8G8B8A8_UNORM;
    return format;
}

void
Texture::Internal::TextureFormatUncompressed::unSwizzle(char* inData, size_t inDataLen, char* outData)
{
    memcpy(outData, inData, inDataLen);
}

Texture::Internal::TextureFormatBC6&
Texture::Internal::TextureFormatBC6::getInstance()
{
    static Texture::Internal::TextureFormatBC6 instance;
    return instance;
}

ddspp::DXGIFormat
Texture::Internal::TextureFormatBC6::format()
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC6H_UF16;
    return format;
}

void
Texture::Internal::TextureFormatBC6::unSwizzle(char* inData, size_t inDataLen, char* outData)
{
    memcpy(outData, inData, inDataLen);
}

Texture::Internal::TextureFormatBC7&
Texture::Internal::TextureFormatBC7::getInstance()
{
    static Texture::Internal::TextureFormatBC7 instance;
    return instance;
}

ddspp::DXGIFormat
Texture::Internal::TextureFormatBC7::format()
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC7_UNORM;
    return format;
}

void
Texture::Internal::TextureFormatBC7::unSwizzle(char* inData, size_t inDataLen, char* outData)
{
    memcpy(outData, inData, inDataLen);
}