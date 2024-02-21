#include "BatchIteratorExport.h"

using namespace WarframeExporter;

BatchIteratorExport::BatchIteratorExport(const std::filesystem::path& pkgsDir, const Ensmallening& ensmalleningData, std::filesystem::path baseOutputPath)
	: BatchIterator(pkgsDir, ensmalleningData, baseOutputPath), m_outExportPath(m_baseOutPath / "Export")
{
}

void
BatchIteratorExport::processKnownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, Extractor* extractor)
{
	std::filesystem::path savePath = m_outExportPath / fileEntry.internalPath.getPreferredPath().relative_path();
	savePath.replace_extension(extractor->getOutputExtension(fileEntry.commonHeader, &fileEntry.headerData));

	if (existingFileIdentical(fileEntry, savePath))
	{
		m_logger.info("Identical file time, skipping: " + fileEntry.internalPath.string());
		return;
	}

	if (!std::filesystem::exists(savePath.parent_path()))
		std::filesystem::create_directories(savePath.parent_path());

	try
	{
		m_logger.info("Extracting " + fileEntry.internalPath.string());
		m_logger.debug(spdlog::fmt_lib::format("Extracting as {}, Enum={}", extractor->getFriendlyName(), fileEntry.commonHeader.type));
		extractor->extract(fileEntry, m_package, m_ensmalleningData, savePath);
		writeFileProperties(savePath, fileEntry);
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
BatchIteratorExport::processUnknownFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry)
{
}

void
BatchIteratorExport::processSkipFile(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const Extractor* extractor)
{
}

bool
BatchIteratorExport::existingFileIdentical(LotusLib::FileEntry& fileEntry, const std::filesystem::path& outputPath)
{
	if (!std::filesystem::exists(outputPath))
		return false;

	FileProperties::TimeDos existingTime = FileProperties::readDos(outputPath, true);

	if (FileProperties::wipeNanoseconds(fileEntry.metadata.getTimeStamp()) != existingTime)
		return false;

	return true;
}

void
BatchIteratorExport::writeFileProperties(const std::filesystem::path filePath, LotusLib::FileEntry& fileEntry)
{
	uint64_t timeRaw = fileEntry.metadata.getTimeStamp();
	FileProperties::writeDos(std::filesystem::path(filePath), timeRaw, timeRaw, timeRaw);
}
