#include "texture/TextureInfos.h"

using namespace WarframeExporter::Texture;

///////////////////////////////////////////////////////////////
// BC1

TextureInfoBC1*
TextureInfoBC1::getInstance()
{
    static TextureInfoBC1* instance = new TextureInfoBC1();
    return instance;
}

std::vector<int>
TextureInfoBC1::getEnumMapKeys() const
{
    static std::vector<int> extTypes = { (int)TextureCompression::BC1, (int)TextureCompression::Default };
    return extTypes;
};

ddspp::DXGIFormat
TextureInfoBC1::getFormat() const
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC1_UNORM;
    return format;
}

void
TextureInfoBC1::unSwizzle(char* inData, size_t inDataLen, char* outData) const
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

///////////////////////////////////////////////////////////////
// BC2

TextureInfoBC2*
TextureInfoBC2::getInstance()
{
    static TextureInfoBC2* instance = new TextureInfoBC2();
    return instance;
}

std::vector<int>
TextureInfoBC2::getEnumMapKeys() const
{
    static std::vector<int> extTypes = { (int)TextureCompression::BC2 };
    return extTypes;
};

ddspp::DXGIFormat
TextureInfoBC2::getFormat() const
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC2_UNORM;
    return format;
}

void
TextureInfoBC2::unSwizzle(char* inData, size_t inDatLen, char* outData)const
{
    char* buffer = new char[inDatLen];
    TextureInfoBC1::getInstance()->unSwizzle(inData, inDatLen, buffer);
    TextureInfoBC1::getInstance()->unSwizzle(buffer, inDatLen, outData);
    delete[] buffer;
}

///////////////////////////////////////////////////////////////
// BC3

TextureInfoBC3*
TextureInfoBC3::getInstance()
{
    static TextureInfoBC3* instance = new TextureInfoBC3();
    return instance;
}

std::vector<int>
TextureInfoBC3::getEnumMapKeys() const
{
    static std::vector<int> extTypes = { (int)TextureCompression::BC3 };
    return extTypes;
};

ddspp::DXGIFormat
TextureInfoBC3::getFormat() const
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC3_UNORM;
    return format;
}

void
TextureInfoBC3::unSwizzle(char* inData, size_t inDatLen, char* outData) const
{
    char* buffer = new char[inDatLen];
    TextureInfoBC1::getInstance()->unSwizzle(inData, inDatLen, buffer);
    TextureInfoBC1::getInstance()->unSwizzle(buffer, inDatLen, outData);
    delete[] buffer;
}

///////////////////////////////////////////////////////////////
// BC4

TextureInfoBC4*
TextureInfoBC4::getInstance()
{
    static TextureInfoBC4* instance = new TextureInfoBC4();
    return instance;
}

std::vector<int>
TextureInfoBC4::getEnumMapKeys() const
{
    static std::vector<int> extTypes = { (int)TextureCompression::BC4 };
    return extTypes;
};

ddspp::DXGIFormat
TextureInfoBC4::getFormat() const
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC4_UNORM;
    return format;
}

void
TextureInfoBC4::unSwizzle(char* inData, size_t inDatLen, char* outData) const
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

///////////////////////////////////////////////////////////////
// BC5

TextureInfoBC5*
TextureInfoBC5::getInstance()
{
    static TextureInfoBC5* instance = new TextureInfoBC5();
    return instance;
}
std::vector<int>
TextureInfoBC5::getEnumMapKeys() const
{
    static std::vector<int> extTypes = { (int)TextureCompression::BC5 };
    return extTypes;
};

ddspp::DXGIFormat
TextureInfoBC5::getFormat() const
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC5_UNORM;
    return format;
}

void
TextureInfoBC5::unSwizzle(char* inData, size_t inDatLen, char* outData) const
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

///////////////////////////////////////////////////////////////
// Uncompressed

TextureInfoUncompressed*
TextureInfoUncompressed::getInstance()
{
    static TextureInfoUncompressed* instance = new TextureInfoUncompressed();
    return instance;
}

std::vector<int>
TextureInfoUncompressed::getEnumMapKeys() const
{
    static std::vector<int> extTypes = { (int)TextureCompression::Uncompressed };
    return extTypes;
};

ddspp::DXGIFormat
TextureInfoUncompressed::getFormat() const
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::R8G8B8A8_UNORM;
    return format;
}

void
TextureInfoUncompressed::unSwizzle(char* inData, size_t inDataLen, char* outData) const
{
    memcpy(outData, inData, inDataLen);
}

///////////////////////////////////////////////////////////////
// BC6

TextureInfoBC6*
TextureInfoBC6::getInstance()
{
    static TextureInfoBC6* instance = new TextureInfoBC6();
    return instance;
}

std::vector<int>
TextureInfoBC6::getEnumMapKeys() const
{
    static std::vector<int> extTypes = { (int)TextureCompression::BC6 };
    return extTypes;
};

ddspp::DXGIFormat
TextureInfoBC6::getFormat() const
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC6H_UF16;
    return format;
}

void
TextureInfoBC6::unSwizzle(char* inData, size_t inDataLen, char* outData) const
{
    memcpy(outData, inData, inDataLen);
}

///////////////////////////////////////////////////////////////
// BC7

TextureInfoBC7*
TextureInfoBC7::getInstance()
{
    static TextureInfoBC7* instance = new TextureInfoBC7();
    return instance;
}

std::vector<int>
TextureInfoBC7::getEnumMapKeys() const
{
    static std::vector<int> extTypes = { (int)TextureCompression::BC7 };
    return extTypes;
};

ddspp::DXGIFormat
TextureInfoBC7::getFormat() const
{
    static ddspp::DXGIFormat format = ddspp::DXGIFormat::BC7_UNORM;
    return format;
}

void
TextureInfoBC7::unSwizzle(char* inData, size_t inDataLen, char* outData) const
{
    memcpy(outData, inData, inDataLen);
}