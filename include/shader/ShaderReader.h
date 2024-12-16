#pragma once

#include "EnumMapValue.h"
#include "BinaryReaderBuffered.h"
#include "shader/ShaderStructs.h"

namespace WarframeExporter::Shader
{
    class ShaderReader : public EnumMapValue
    {
    public:
        virtual ShaderHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* headerReader, int shaderTypeEnum) = 0;
        virtual ShaderEntry readShader(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index) = 0;
        virtual std::vector<ShaderEntry> readAllShaders(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader) = 0;

        void readShaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, BinaryReader::BinaryReaderBuffered* bodyReader);
    };
};