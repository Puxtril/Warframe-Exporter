#pragma once

#include "Extractor.h"
#include "shader/ShaderStructs.h"
#include "shader/ShaderTypes.h"
#include "shader/ShaderEnumMap.h"
#include "shader/ShaderConverter.h"
#include "shader/ShaderConverterD3D.h"
#include "shader/ShaderExportType.h"

namespace WarframeExporter::Shader
{
    class ShaderExtractor : public Extractor
    {
		bool m_hasWarnedCompileNonWindows;
		
        ShaderExtractor();

    public:
        ShaderExtractor(const ShaderExtractor&) = delete;
		ShaderExtractor operator=(const ShaderExtractor&) = delete;

		static inline ShaderExportType m_shaderExportType;

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
		
		inline bool isMultiExport() const override
		{
			return true;
		}

		inline ExtractorType getExtractorType() const override
        {
            static ExtractorType type = ExtractorType::Shader;
			return type;
        }

        inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			const static std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::SHADER_PERMUTATION, (int)ShaderType::SHADER_18 },
				//{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::SHADER_PERMUTATION, (int)ShaderType::SHADER_21 },
				//{ LotusLib::Game::SOULFRAME, LotusLib::PackageCategory::SHADER_PERMUTATION, (int)ShaderType::SHADER_21 },
				//{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::SHADER, (int)ShaderType::SHADER_22 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::SHADER_PERMUTATION, (int)ShaderType::SHADER_23 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::SHADER_PERMUTATION, (int)ShaderType::SHADER_24 },
				{ LotusLib::Game::WARFRAME, LotusLib::PackageCategory::SHADER_PERMUTATION, (int)ShaderType::SHADER_26 },
			};
			return extTypes;
		}

        static ShaderExtractor* getInstance();

        ShaderHeaderExternal getHeader(LotusLib::FileEntry& fileEntry);

        ShaderEntry readEntry(LotusLib::FileEntry& fileEntry, const ShaderHeaderExternal& header, int index);
        std::vector<ShaderEntry> readAllEntries(LotusLib::FileEntry& fileEntry, const ShaderHeaderExternal& header);
		
		void decompileShader(ShaderEntry& shaderEntry);
		void decompileShaders(std::vector<ShaderEntry>& shaderEntries);

        void writeShader(const ShaderEntry& shader, const std::filesystem::path& outputDir, int shaderIndex = 0);

		void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputDir, bool dryRun = false) override;
		
	private:
		void _decompileShader(ShaderEntry& shaderEntry, int index = -1);
    };
}