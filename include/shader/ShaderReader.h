#pragma once

#include "EnumMap.h"
#include "BinaryReader/Buffered.h"
#include "shader/ShaderStructs.h"

namespace WarframeExporter::Shader
{
    class ShaderReader : public EnumMapValue
    {
    public:
        virtual ShaderHeaderExternal readHeader(BinaryReader::Buffered* headerReader, int shaderTypeEnum) = 0;
        virtual ShaderEntry readShader(BinaryReader::Buffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index) = 0;
        virtual std::vector<ShaderEntry> readAllShaders(BinaryReader::Buffered* bodyReader, const ShaderHeaderExternal& shaderHeader) = 0;

        void readShaderDebug(BinaryReader::Buffered* headerReader, BinaryReader::Buffered* bodyReader);
    };
};