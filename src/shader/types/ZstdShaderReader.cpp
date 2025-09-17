#include "shader/types/ZstdShaderReader.h"

using namespace WarframeExporter::Shader;

ZstdShaderReader*
ZstdShaderReader::getInstance()
{
    static ZstdShaderReader instance;
    return &instance;
}

void
ZstdShaderReader::initilizeDecompressor(LotusLib::PackagesReader& pkgs)
{
    m_zstdDecompressor.initilize(pkgs);
}

ShaderHeaderExternal
ZstdShaderReader::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, int shaderTypeEnum)
{
    ShaderHeaderExternal shaderHeader;

    if (shaderTypeEnum >= 23)
    {
        headerReader->seek(8, std::ios::cur);

        // Techrot Encore added another 8 bytes to this section
        // Luckily all shaders have an int32 of value -1 that we can anchor with

        // Pre-Techrot
        if (headerReader->readInt32() == -1)
        {
            headerReader->seek(10, std::ios::cur);
        }
        // Post-Techrot
        else
        {
            headerReader->seek(4, std::ios::cur);
            if (headerReader->readInt32() == -1)
                headerReader->seek(8, std::ios::cur);
            else
                throw unknown_format_error("Unknown shader header");
        }
        
    }
    else
        headerReader->seek(8, std::ios::cur);

    uint32_t shaderPathLen = headerReader->readUInt32();
    headerReader->seek(shaderPathLen, std::ios::cur);

    headerReader->seek(18, std::ios::cur);

    shaderHeader.shaderCount = headerReader->readUInt32();

    headerReader->seek(22, std::ios::cur);

    shaderHeader.shaderCodeTotalSize = headerReader->readUInt32();

    headerReader->seek(28, std::ios::cur);

    uint32_t shaderLengthsLen = headerReader->readUInt32();
    shaderHeader.shaderLengths.resize(shaderLengthsLen);
    headerReader->readUInt32Array(shaderHeader.shaderLengths.data(), shaderLengthsLen);

    return shaderHeader;
}

ShaderEntry
ZstdShaderReader::readShader(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index)
{
    uint32_t firstShaderOffset = static_cast<uint32_t>(bodyReader->getLength()) - shaderHeader.shaderCodeTotalSize;
    bodyReader->seek(firstShaderOffset, std::ios::beg);

    for (int iShader = 0; iShader < index; iShader++)
    {
        bodyReader->seek(shaderHeader.shaderLengths[iShader], std::ios::cur);
    }

    ShaderEntry shader;

    int bytecodeSize = shaderHeader.shaderLengths[index] - 20;
    shader.bytecode.resize(bytecodeSize);
    bodyReader->readUInt8Array((uint8_t*)shader.bytecode.data(), bytecodeSize);

    bodyReader->seek(2, std::ios::cur);
    
    shader.stage = bodyReader->readUInt16();

    return shader;
}

std::vector<ShaderEntry>
ZstdShaderReader::readAllShaders(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader)
{
    static std::vector<uint8_t> bufferDecompress(10485760); // 10MB

    const size_t beginningUnkBytes = (shaderHeader.shaderCount * 8) + 2;
    const size_t adjustedShaderBytecodeLength = shaderHeader.shaderCodeTotalSize + (shaderHeader.shaderCount * 8);
    const size_t endPadding = shaderHeader.shaderCount * 12;
    const size_t firstShaderOffset = bodyReader->getLength() - endPadding - adjustedShaderBytecodeLength + beginningUnkBytes;
    bodyReader->seek(firstShaderOffset, std::ios::beg);

    std::vector<ShaderEntry> shaders(shaderHeader.shaderCount);

    int skipCount = 0;
    int dictIndex = -1;
    for (size_t iShader = 0; iShader < shaderHeader.shaderCount; iShader++)
    {
        ShaderEntry& curShader = shaders[iShader];

        int bytecodeSize = shaderHeader.shaderLengths[iShader] - 2;
        const unsigned char* bytecodePtr = bodyReader->getPtr().data() + bodyReader->tell();
        bodyReader->seek(bytecodeSize + 2, std::ios::cur);

        // Decompression method currently unknown
        // This is DictionaryIdFlag (3 bits) inside Zstd blocks
        if (bytecodePtr[0] & 3)
        {
            skipCount++;
            continue;
        }
        
        if (dictIndex == -1)
        {
            for (int i = 0; i < 3; i++)
            {
                size_t errorCheck = m_zstdDecompressor.decompress((const char*)bytecodePtr, bytecodeSize, (char*)bufferDecompress.data(), bufferDecompress.capacity(), i);
                if (!ZSTD_isError(errorCheck))
                {
                    dictIndex = i;
                    break;
                }
            }
            if (dictIndex == -1)
                throw unknown_format_error("Unable to determine zstd dictionary");
        }

        size_t decompressedLen = m_zstdDecompressor.decompress((const char*)bytecodePtr, bytecodeSize, (char*)bufferDecompress.data(), bufferDecompress.capacity(), dictIndex);
        if (ZSTD_isError(decompressedLen))
            throw unknown_format_error("Error with ZSTD decompression: " + std::string(ZSTD_getErrorName(decompressedLen)));

        curShader.bytecode.resize(decompressedLen + 4);
        // Magic bytes DXBC
        curShader.bytecode[0] = 0x44;
        curShader.bytecode[1] = 0x58;
        curShader.bytecode[2] = 0x42;
        curShader.bytecode[3] = 0x43;
        memcpy(curShader.bytecode.data() + 4, bufferDecompress.data(), decompressedLen);
    }

    WarframeExporter::Logger::getInstance().info("Skipped " + std::to_string(skipCount) + "/" + std::to_string(shaderHeader.shaderCount) + " shaders");
    return shaders;
}