#pragma once

#include "shader/ShaderReader.h"
#include "shader/ShaderTypes.h"
#include "ExporterExceptions.h"
#include "shader/ShaderDecompressZstd.h"

namespace WarframeExporter::Shader
{
    class ZstdShaderReader : public WarframeExporter::Shader::ShaderReader
    {
        WarframeDecompressZstd m_zstdDecompressor;

    public:
        static ZstdShaderReader* getInstance();

        inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
                (int)ShaderType::SHADER_24,
                (int)ShaderType::SHADER_25,
            };
			return extTypes;
		}

        void initilizeDecompressor(const LotusLib::PackageCollection& pkgs);
        ShaderHeaderExternal readHeader(BinaryReader::Buffered* headerReader, int shaderTypeEnum) override;
        ShaderEntry readShader(BinaryReader::Buffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index) override;
        std::vector<ShaderEntry> readAllShaders(BinaryReader::Buffered* bodyReader, const ShaderHeaderExternal& shaderHeader) override;
    };
};