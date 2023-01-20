#pragma once

#include "../BaseExtractor.h"
#include "../Ensmallening.hpp"
#include "ModelConverter.h"
#include "GltfExporter.h"
#include "BinaryReaderBuffered.h"
#include "ModelStructs.hpp"
#include "types/ModelReader269.hpp"
#include "types/ModelReader272.hpp"
#include "types/ModelReader96.hpp"
#include "model/VertexColor.h"

#include <iostream>

namespace WarframeExporter
{
	namespace Model
	{
		class ExtractorModel : public Extractor
		{
		public:
			inline std::string outFileExt()
			{
				static std::string outFileExt = "glb";
				return outFileExt;
			}

			inline std::string friendlyName()
			{
				static std::string friendlyName = "3DModel";
				return friendlyName;
			}

			inline std::vector<FileTypeExternal> externalTypes()
			{
				std::vector<FileTypeExternal> extTypes = {
					FileTypeExternal::MODEL_STATIC_96,
					FileTypeExternal::MODEL_RIGGED_269,
					FileTypeExternal::MODEL_RIGGED_272
				};
				return extTypes;
			}

			inline FileTypeInternal internalType()
			{
				static FileTypeInternal type = FileTypeInternal::Model;
				return type;
			}

			static ExtractorModel& getInstance();

			void extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath) override;
			void extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData) override;
		};
	}
}