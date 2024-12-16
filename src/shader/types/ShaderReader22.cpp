#include "shader/types/ShaderReader22.h"

using namespace WarframeExporter::Shader;

ShaderReader22*
ShaderReader22::getInstance()
{
    static ShaderReader22 instance;
    return &instance;
}

ShaderHeaderExternal
ShaderReader22::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, int shaderEnumType)
{
    // Intentional
    // I don't know what to do with this data
    ShaderHeaderExternal header;
    return header;

    headerReader->seek(9, std::ios::cur);

    const static std::string attrFirstMsg = "First 3 attribute arrays";
    skipStringArray(headerReader, attrFirstMsg);
    skipStringArray(headerReader, attrFirstMsg);
    skipStringArray(headerReader, attrFirstMsg);

    const static std::string packaged1Msg = "First packagedAttribute";
    skipAttributePackaged(headerReader, packaged1Msg);

    const static std::string packaged2Msg = "Seccond packagedAttribute";
    skipAttributePackaged(headerReader, packaged2Msg);

    headerReader->seek(4, std::ios::cur);

    uint32_t skip24Len = headerReader->readUInt32();
    headerReader->seek(24 * skip24Len, std::ios::cur);

    uint32_t attributePadCount = headerReader->readUInt32();
    for (uint32_t i = 0; i < attributePadCount; i++)
    {
        uint32_t nameLen = headerReader->readUInt32();
        headerReader->seek(nameLen, std::ios::cur);

        uint32_t idkAttributeLen = headerReader->readUInt32();
        headerReader->seek(idkAttributeLen * (6 * 4), std::ios::cur);
    }

    // !
    uint32_t attributeArrayArraylen = headerReader->readUInt32();
    for (uint32_t i = 0; i < attributeArrayArraylen * 2; i++)
    {
        const std::string attributeArrayArrayMsg = "Attribute Array Array";
        skipStringArray(headerReader, attributeArrayArrayMsg);
    }

    uint32_t attribute2Len = headerReader->readUInt32();
    for (uint32_t i = 0; i < attribute2Len; i++)
    {
        const std::string attribute2Msg = "Attribute2 Array Name";
        uint32_t nameLen = headerReader->readUInt32(0, 1000, attribute2Msg);
        headerReader->seek(nameLen, std::ios::cur);

        headerReader->seek(24, std::ios::cur);
    }

    // Hardcoded 4?
    for (int i = 0; i < 4; i++)
    {
        uint32_t ShaderAttributeLen = headerReader->readUInt32();
        for (uint32_t i = 0; i < ShaderAttributeLen; i++)
        {
            headerReader->seek(48, std::ios::cur);

            uint32_t helplen = headerReader->readUInt32();
            headerReader->seek(helplen, std::ios::cur);

            headerReader->seek(12, std::ios::cur);

            uint32_t untintableLen = headerReader->readUInt32();
            headerReader->seek(untintableLen, std::ios::cur);

            headerReader->seek(34, std::ios::cur);

            uint32_t nameLen = headerReader->readUInt32();
            headerReader->seek(nameLen, std::ios::cur);
        }

        uint32_t lightmapEntryLen = headerReader->readUInt32();
        for (uint32_t i = 0; i < lightmapEntryLen; i++)
        {
            headerReader->seek(33, std::ios::cur);

            uint32_t material1NameLen = headerReader->readUInt32();
            headerReader->seek(material1NameLen, std::ios::cur);

            uint32_t material2NameLen = headerReader->readUInt32();
            headerReader->seek(material2NameLen, std::ios::cur);

            headerReader->seek(2, std::ios::cur);

            uint32_t nameLen = headerReader->readUInt32();
            headerReader->seek(nameLen, std::ios::cur);
        }
    }

    int32_t shaderBytecodeCheck = headerReader->readInt32();
    if (shaderBytecodeCheck != -1)
    {
        headerReader->seek(59, std::ios::cur);

        header.shaderCodeTotalSize = headerReader->readUInt32();

        const static std::string shaderCodeSize = "Shader count";
        header.shaderCount = headerReader->readUInt32(0, 10000, shaderCodeSize);

        header.shaderLengths.resize(header.shaderCount);
        headerReader->readUInt32Array(header.shaderLengths.data(), header.shaderCount);
    }
    // No shader bytecode, let's confirm
    // There should only be 75 bytes left in the file
    else
    {
        int remainingBytes = (int)headerReader->getLength() - (int)headerReader->tell();
        if (remainingBytes > 80)
            throw unknown_format_error("No shader bytecode, but more data exists");
        
        header.shaderCodeTotalSize = 0;
        header.shaderCount = 0;
    }

    return header;
}

void
ShaderReader22::skipStringArray(BinaryReader::BinaryReaderBuffered* headerReader, const std::string& msg)
{
    uint32_t arrLen = headerReader->readUInt32(0, 500, msg);
    for (uint32_t i = 0; i < arrLen; i++)
    {
        uint32_t nameLen = headerReader->readUInt32(0, 500, msg);
        headerReader->seek(nameLen, std::ios::cur);
    }
}

void
ShaderReader22::skipAttributePackaged(BinaryReader::BinaryReaderBuffered* headerReader, const std::string& msg)
{
    uint32_t attributePackageCount = headerReader->readUInt32();
    for (uint32_t i = 0; i < attributePackageCount; i++)
    {
        uint32_t nameLen = headerReader->readUInt32();
        headerReader->seek(nameLen, std::ios::cur);

        skipStringArray(headerReader, msg);

        headerReader->seek(1, std::ios::cur);
    }
}