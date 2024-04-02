#include "shader/types/StandardShaderReader.h"

using namespace WarframeExporter::Shader;

StandardShaderReader*
StandardShaderReader::getInstance()
{
    static StandardShaderReader instance;
    return &instance;
}

ShaderHeaderExternal
StandardShaderReader::readHeader(BinaryReader::BinaryReaderBuffered* headerReader)
{
    ShaderHeaderExternal shaderHeader;

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
StandardShaderReader::readShader(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index)
{
    int firstShaderOffset = bodyReader->getLength() - shaderHeader.shaderCodeTotalSize;
    bodyReader->seek(firstShaderOffset, std::ios::beg);

    for (int iShader = 0; iShader < index; iShader++)
    {
        bodyReader->seek(shaderHeader.shaderLengths[iShader], std::ios::cur);
    }

    ShaderEntry shader;

    int bytecodeSize = shaderHeader.shaderLengths[index] - 18;
    shader.bytecode.resize(bytecodeSize);
    bodyReader->readUInt8Array((uint8_t*)shader.bytecode.data(), bytecodeSize);

    bodyReader->seek(2, std::ios::cur);
    
    shader.stage = bodyReader->readUInt16();

    return shader;
}

std::vector<ShaderEntry>
StandardShaderReader::readAllShaders(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader)
{
    int firstShaderOffset = bodyReader->getLength() - shaderHeader.shaderCodeTotalSize;
    bodyReader->seek(firstShaderOffset, std::ios::beg);

    std::vector<ShaderEntry> shaders(shaderHeader.shaderCount);

    for (size_t iShader = 0; iShader < shaderHeader.shaderCount; iShader++)
    {
        ShaderEntry& curShader = shaders[iShader];

        int bytecodeSize = shaderHeader.shaderLengths[iShader] - 18;
        curShader.bytecode.resize(bytecodeSize);
        bodyReader->readUInt8Array((uint8_t*)curShader.bytecode.data(), bytecodeSize);

        bodyReader->seek(2, std::ios::cur);
        
        curShader.stage = bodyReader->readUInt16();

        bodyReader->seek(14, std::ios::cur);
    }

    return shaders;
}

ShaderHeaderExternal
StandardShaderReader::readShaderHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader)
{
    ShaderHeaderExternal shaderHeader;

    headerReader->seek(8, std::ios::cur);

    static const std::string headerFirstShaderPathLen = "Header first shader path";
    uint32_t shaderPathLen = headerReader->readUInt32(0, 1000, headerFirstShaderPathLen);
    headerReader->seek(shaderPathLen, std::ios::cur);

    headerReader->seek(18, std::ios::cur);

    static const std::string headerShaderCount = "Header shader count";
    shaderHeader.shaderCount = headerReader->readUInt32(0, 10000, headerShaderCount);

    headerReader->seek(22, std::ios::cur);

    // 100MB
    const static std::string shaderCodeLen = "Shader code length";
    shaderHeader.shaderCodeTotalSize = headerReader->readUInt32(0, 100000000, shaderCodeLen);

    headerReader->seek(28, std::ios::cur);

    const static std::string shaderLengthsCount = "Shader lengths array length";
    uint32_t shaderLengthsLen = headerReader->readUInt32(0, 100000000, shaderLengthsCount);
    shaderHeader.shaderLengths.resize(shaderLengthsLen);
    headerReader->readUInt32Array(shaderHeader.shaderLengths.data(), shaderLengthsLen);

    return shaderHeader;
}

void
StandardShaderReader::readShaderBodyDebug(const ShaderHeaderExternal& shaderHeader, BinaryReader::BinaryReaderBuffered* bodyReader)
{
    int firstShaderOffset = bodyReader->getLength() - shaderHeader.shaderCodeTotalSize;
    bodyReader->seek(firstShaderOffset, std::ios::beg);

    for (size_t iShader = 0; iShader < shaderHeader.shaderCount; iShader++)
    {
        int shaderSize = shaderHeader.shaderLengths[iShader] - 18;
        bodyReader->seek(shaderSize, std::ios::cur);

        bodyReader->seek(2, std::ios::cur);
        
        //std::string shaderTypeMsg = "Shader stage enumeration " + std::to_string(iShader);
        //bodyReader->readUInt16(0, 1, shaderTypeMsg);
        bodyReader->readUInt16();

        bodyReader->seek(14, std::ios::cur);
    }
}
