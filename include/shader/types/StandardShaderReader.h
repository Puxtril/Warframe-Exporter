#pragma once

#include "shader/ShaderReader.h"
#include "shader/ShaderTypes.h"
#include "ExporterExceptions.h"

namespace WarframeExporter::Shader
{
    class StandardShaderReader : public WarframeExporter::Shader::ShaderReader
    {
    public:
        static StandardShaderReader* getInstance();

        inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
                (int)ShaderType::SHADER_18,
                (int)ShaderType::SHADER_21,
                (int)ShaderType::SHADER_23,
                (int)ShaderType::SHADER_26,
            };
			return extTypes;
		}

        ShaderHeaderExternal readHeader(BinaryReader::Buffered* headerReader, int shaderTypeEnum) override;
        ShaderEntry readShader(BinaryReader::Buffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index) override;
        std::vector<ShaderEntry> readAllShaders(BinaryReader::Buffered* bodyReader, const ShaderHeaderExternal& shaderHeader) override;
    };
};