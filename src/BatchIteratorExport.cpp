#include "BatchIteratorExport.h"

using namespace WarframeExporter;

BatchIteratorExport::BatchIteratorExport()
	: BatchIterator()
{
}

void
BatchIteratorExport::processKnownFile(LotusLib::PackagesReader& pkgs, LotusLib::FileEntry& fileEntry, Extractor* extractor, const std::filesystem::path& outputPath, ExtractOptions options)
{
	std::filesystem::path savePath;

	if (extractor->isMultiExport())
	{
		savePath = outputPath / fileEntry.internalPath.relative_path();
		savePath.replace_extension("");

		if (!std::filesystem::exists(savePath) && !options.dryRun)
			std::filesystem::create_directories(savePath);
	}
	else
	{
		savePath = outputPath / fileEntry.internalPath.relative_path();
		savePath.replace_extension(extractor->getOutputExtension(fileEntry.commonHeader, &fileEntry.headerData));

	if (existingFileIdentical(fileEntry.metadata->getTimeStamp(), savePath))
	{
		m_logger.info("Identical file time, skipping: " + fileEntry.internalPath.string());
		return;
	}

		if (!std::filesystem::exists(savePath.parent_path()) && !options.dryRun)
			std::filesystem::create_directories(savePath.parent_path());
	}

	try
	{
		LotusLib::FileEntry fullFileEntry = pkgs.getPackage(fileEntry.srcPkgName).value().getFile(fileEntry.metadata);
		
		m_logger.info("Extracting " + fullFileEntry.internalPath.string());
		m_logger.debug(spdlog::fmt_lib::format("Extracting as {}, Enum={}", extractor->getFriendlyName(), fullFileEntry.commonHeader.type));

		extractor->extract(fullFileEntry, pkgs, savePath, options);
		writeFileProperties(savePath, fullFileEntry);
	}
	catch (not_imeplemented_error& err)
	{
		m_logger.warn("Not implemented: " + std::string(err.what()) + " " + fileEntry.internalPath.string());
	}
	catch (unknown_format_error& err)
	{
		m_logger.error("Unknown Format: " + std::string(err.what()) + " " + fileEntry.internalPath.string());
	}
	catch (std::exception& err)
	{
		m_logger.error(std::string(err.what()) + ": " + fileEntry.internalPath.string());
	}
}

void
BatchIteratorExport::processUnknownFile(LotusLib::PackagesReader& pkgs, LotusLib::FileEntry& fileEntry, const std::filesystem::path& outputPath)
{
}

void
BatchIteratorExport::processSkipFile(LotusLib::PackagesReader& pkgs, LotusLib::FileEntry& fileEntry, const Extractor* extractor)
{
}

bool
BatchIteratorExport::existingFileIdentical(int64_t timestamp, const std::filesystem::path& outputPath)
{
	if (!std::filesystem::exists(outputPath))
		return false;

	FileProperties::TimeDos existingTime = FileProperties::readDos(outputPath, true);

	if (FileProperties::wipeNanoseconds(timestamp) != existingTime)
		return false;

	return true;
}

void
BatchIteratorExport::writeFileProperties(const std::filesystem::path filePath, LotusLib::FileEntry& fileEntry)
{
	uint64_t timeRaw = fileEntry.metadata->getTimeStamp();
	FileProperties::writeDos(std::filesystem::path(filePath), timeRaw, timeRaw, timeRaw);
}
