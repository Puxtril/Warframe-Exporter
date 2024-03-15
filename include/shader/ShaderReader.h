#pragma once

#include "EnumMapValue.h"
#include "BinaryReaderBuffered.h"
#include "shader/ShaderStructs.h"

namespace WarframeExporter::Shader
{
    class ShaderReader : public EnumMapValue
    {
    public:
        virtual ShaderHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* headerReader) = 0;
        virtual ShaderEntry readShader(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index) = 0;
        virtual std::vector<ShaderEntry> readAllShaders(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader) = 0;

        void readShaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, BinaryReader::BinaryReaderBuffered* bodyReader);

    protected:
        virtual ShaderHeaderExternal readShaderHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader) = 0;
        virtual void readShaderBodyDebug(const ShaderHeaderExternal& shaderHeader, BinaryReader::BinaryReaderBuffered* bodyReader) = 0;
    };
};