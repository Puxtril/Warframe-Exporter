#pragma once

#include "shader/ShaderReader.h"
#include "shader/types/StandardShaderReader.h"
#include "shader/ShaderTypes.h"
#include "ExporterExceptions.h"

namespace WarframeExporter::Shader
{
    // The only difference with 22 is the densly populated header.
    // The body is identical
    class ShaderReader22 : public StandardShaderReader
    {
    public:
        static ShaderReader22* getInstance();

        inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
                (int)ShaderType::SHADER_22
            };
			return extTypes;
		}

        ShaderHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* headerReader, int shaderEnumType) override;
    
    protected:
        void skipStringArray(BinaryReader::BinaryReaderBuffered* headerReader, const std::string& msg);
        void skipAttributePackaged(BinaryReader::BinaryReaderBuffered* headerReader, const std::string& msg);
    };
};