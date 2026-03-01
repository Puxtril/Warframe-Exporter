#pragma once

#include "Extractor.h"
#include "BinaryReader/Buffered.h"
#include "LotusLib/Enums.h"

namespace WarframeExporter::Icon
{
	// Unfortunately this format overlaps with another enum - the manifest.
	// If the return type of `getExternalIcon` is of size 0, then it's a manifest.
	class IconExtractor : public Extractor
	{
		IconExtractor() : Extractor() {}
		
	public:
		IconExtractor(const IconExtractor&) = delete;
		IconExtractor operator=(const IconExtractor&) = delete;

		inline const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::Buffered* hReader, WarframeExporter::ExtractOptions options) const override
		{
			const static std::string& txtExt = "ico";
			return txtExt;

		}
		
		inline bool isMultiExport() const override
		{
			return false;
		}

		inline const std::string& getFriendlyName() const override
		{
			const static std::string friendlyName = "Icon";
			return friendlyName;
		}

		inline ExtractorType getExtractorType() const override
		{
			static ExtractorType type = ExtractorType::Icon;
			return type;
		}

		inline std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const override
		{
			const static std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> extTypes = {
				{LotusLib::Game::WARFRAME, LotusLib::PackageCategory::MISC, 3}
			};
			return extTypes;
		}

		static IconExtractor* getInstance();

		std::vector<uint8_t> getExternalIcon(LotusLib::FileEntry& fileEntry);
		void extract(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const ExtractOptions options) override;
	};
}
