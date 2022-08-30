#pragma once

#include "BinaryReaderBuffered.h"
#include "CommonHeader.h"
#include "Ensmallening.hpp"
#include "FileType.hpp"
#include "CacheReaderLimited.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/fmt.h"

#include <vector>
#include <string>

namespace WarframeExporter
{
	class Extractor
	{
	protected:
		std::shared_ptr<spdlog::logger> m_logger;

		Extractor();

		Extractor(const Extractor& other) = delete;
		Extractor& operator=(const Extractor& other) = delete;

	public:
		virtual std::string outFileExt() = 0;
		virtual std::string friendlyName() = 0;
		virtual std::vector<FileTypeExternal> externalTypes() = 0;
		virtual FileTypeInternal internalType() = 0;

		// virtual static Extractor& getInstance();
		// virtual HeaderStruct readHeader	(BinaryReaderBuffered* HfileReader, BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header);
		// virtual BodyStruct	readBody	(BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader, const CommonFileHeader& header, const HeaderStruct& headerInternal);
		// virtual void			writeData	(const std::string& outputFile, const HeaderStruct& header, const BodyStruct& body, const CommonFileHeader& comHeader);

		//virtual void extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, BinaryReaderBuffered* bReader, BinaryReaderBuffered* fReader, const Ensmallening& ensmalleningData, const std::string& outputPath) = 0;
		//virtual void extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, BinaryReaderBuffered* bReader, BinaryReaderBuffered* fReader, const Ensmallening& ensmalleningData) = 0;
		virtual void extract(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData, const std::string& outputPath) = 0;
		virtual void extractDebug(const CommonFileHeader& header, BinaryReaderBuffered* hReader, PackageDirLimited& cacheDir, const std::string& package, const std::string& internalpath, const Ensmallening& ensmalleningData) = 0;
	};
}