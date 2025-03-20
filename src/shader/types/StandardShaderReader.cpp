#include "shader/types/StandardShaderReader.h"

using namespace WarframeExporter::Shader;

StandardShaderReader*
StandardShaderReader::getInstance()
{
    static StandardShaderReader instance;
    return &instance;
}

ShaderHeaderExternal
StandardShaderReader::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, int shaderTypeEnum)
{
    ShaderHeaderExternal shaderHeader;

    if (shaderTypeEnum == 23)
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
StandardShaderReader::readShader(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index)
{
    uint32_t firstShaderOffset = static_cast<uint32_t>(bodyReader->getLength()) - shaderHeader.shaderCodeTotalSize;
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
    uint32_t firstShaderOffset = static_cast<uint32_t>(bodyReader->getLength()) - shaderHeader.shaderCodeTotalSize;
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
