#pragma once

#include "LotusLib.h"
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
            };
			return extTypes;
		}

        void initilizeDecompressor(LotusLib::PackagesReader& pkgs);
        ShaderHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* headerReader, int shaderTypeEnum) override;
        ShaderEntry readShader(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader, int index) override;
        std::vector<ShaderEntry> readAllShaders(BinaryReader::BinaryReaderBuffered* bodyReader, const ShaderHeaderExternal& shaderHeader) override;
    };
};