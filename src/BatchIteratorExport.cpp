#include "BatchIteratorExport.h"

using namespace WarframeExporter;

BatchIteratorExport::BatchIteratorExport(LotusLib::PackageCollection<LotusLib::CachePairReader>* package, const Ensmallening& ensmalleningData, std::string baseOutputPath)
	: BatchIterator(package, ensmalleningData, baseOutputPath)
{
}

void
BatchIteratorExport::processKnownFile(const std::string& packageName, const std::string& internalPath, BinaryReaderBuffered* hReader, const LotusLib::CommonHeader& header, Extractor* extractor)
{
	std::string outputPath = m_pathManager.getOutputFilePath(internalPath, extractor->getOutputExtension());
	if (existingFileIdentical(internalPath, outputPath, (*m_package)[packageName][LotusLib::PackageTrioType::H], packageName))
	{
		m_logger.info("Identical file time, skipping: " + internalPath);
		return;
	}

	try
	{
		m_logger.info("Extracting " + internalPath);
		m_logger.debug(spdlog::fmt_lib::format("Extracting as {}, Enum={}", extractor->getFriendlyName(), header.type));
		extractor->extract(header, hReader, *m_package, packageName, internalPath, m_ensmalleningData, outputPath);
		writeFileProperties(outputPath, internalPath, packageName);
	}
	catch (not_imeplemented_error& err)
	{
		m_logger.debug("Not implemented: " + std::string(err.what()) + " " + internalPath);
	}
	catch (unknown_format_error& err)
	{
		m_logger.debug("Unknown Format: " + std::string(err.what()) + " " + internalPath);
	}
	catch (std::exception& err)
	{
		m_logger.error(std::string(err.what()) + ": " + internalPath);
	}
}

void
BatchIteratorExport::processUnknownFile(const std::string& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file)
{
}

void
BatchIteratorExport::processSkipFile(const std::string& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file, const Extractor* extractor)
{
}

bool
BatchIteratorExport::existingFileIdentical(const std::string& internalPath, const std::string& outputPath, const std::shared_ptr<LotusLib::CachePairReader> curPair, const std::string& packageName)
{
	const LotusLib::FileEntries::FileNode* internalNode = curPair->getFileEntry(internalPath);
	std::filesystem::path p(outputPath);
	
	if (!std::filesystem::exists(p))
		return false;

	int64_t existingTime = FileProperties::readWinTimeMod(p);
	// TODO: Update timestamp to int64_t
	if ((int64_t)internalNode->getTimeStamp() != existingTime)
		return false;

	return true;
}

void
BatchIteratorExport::writeFileProperties(const std::string filePath, const std::string internalPath, const std::string& packageName)
{
	std::shared_ptr<LotusLib::CachePairReader> pkg = (*m_package)[packageName][LotusLib::PackageTrioType::H];
	if (pkg != nullptr)
	{
		const LotusLib::FileEntries::FileNode* fileNode = pkg->getFileEntry(internalPath);
		uint64_t timeRaw = fileNode->getTimeStamp();
		FileProperties::writeWinTime(std::filesystem::path(filePath), timeRaw, timeRaw, timeRaw);
	}
}
