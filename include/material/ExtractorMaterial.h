#pragma once

#include "../BaseExtractor.h"
#include "BinaryReaderBuffered.h"
#include "../Ensmallening.hpp"

namespace WarframeExporter
{
	namespace Material
	{
		class ExtractorMaterial : public Extractor
		{
		public:
			inline std::string outFileExt()
			{
				const static std::string outFileExt = "txt";
				return outFileExt;
			}

			inline std::string friendlyName()
			{
				const static std::string friendlyName = "Material";
				return friendlyName;
			}

			inline std::vector<FileTypeExternal> externalTypes()
			{
				const static std::vector<FileTypeExternal> extTypes = {
					FileTypeExternal::MATERIAL_203,
					FileTypeExternal::MATERIAL_204,
					FileTypeExternal::MATERIAL_205
				};
				return extTypes;
			}

			inline FileTypeInternal internalType()
			{
				static FileTypeInternal type = FileTypeInternal::Material;
				return type;
			}

			static ExtractorMaterial& getInstance();

			void getExtraNames(BinaryReaderBuffered* headerReader, std::vector<std::string>& outPaths);
			void extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath) override;
			void extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData) override;
		};
	}
}