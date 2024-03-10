#pragma once

#include "Extractor.h"

namespace WarframeExporter::Shader
{
    enum class ShaderType
    {
        SHADER_18 = 18,
        SHADER_21 = 21,
        SHADER_26 = 26
    };

    class ShaderExtractor : public Extractor
    {
        ShaderExtractor() : Extractor() {}

    public:
        ShaderExtractor(const ShaderExtractor&) = delete;
		ShaderExtractor operator=(const ShaderExtractor&) = delete;

        inline const std::string& getFriendlyName() const override
        {
            const static std::string friendlyName = "Shader";
			return friendlyName;
        }

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader) const override
        {
            const static std::string ext = "hlsl";
			return ext;
        }

		inline ExtractorType getExtractorType() const override
        {
            static ExtractorType type = ExtractorType::Shader;
			return type;
        }

        inline std::vector<int> getEnumMapKeys() const override
		{
			const static std::vector<int> extTypes = {
				(int)ShaderType::SHADER_18,
				(int)ShaderType::SHADER_21,
				(int)ShaderType::SHADER_26,
			};
			return extTypes;
		}

        static ShaderExtractor* getInstance();

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath) override;
		void extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData) override;
    };
}