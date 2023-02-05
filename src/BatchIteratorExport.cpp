#include "BatchIteratorExport.h"

using namespace WarframeExporter;

BatchIteratorExport::BatchIteratorExport(PackageReader::PackageDir* package, const Ensmallening& ensmalleningData, std::string baseOutputPath)
	: BatchIterator(package, ensmalleningData, baseOutputPath)
{
}

void
BatchIteratorExport::processKnownFile(PackageDirLimited& pkgParam, const std::string& packageName, const std::string& internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, Extractor* extractor)
{
	std::string outputPath = m_pathManager.getOutputFilePath(internalPath, extractor->getOutputExtension());
	if (existingFileIdentical(internalPath, outputPath, (*m_package)[packageName][PackageReader::PackageTrioType::H], packageName))
	{
		m_logger.info("Identical file time, skipping: " + internalPath);
		return;
	}

	try
	{
		m_logger.info("Extracting " + internalPath);
		m_logger.debug(spdlog::fmt_lib::format("Extracting as {}, Enum={}", extractor->getFriendlyName(), header.getEnum()));
		extractor->extract(header, hReader, pkgParam, packageName, internalPath, m_ensmalleningData, outputPath);
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
BatchIteratorExport::processUnknownFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file)
{
}

void
BatchIteratorExport::processSkipFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file, const Extractor* extractor)
{
}

bool
BatchIteratorExport::existingFileIdentical(const std::string& internalPath, const std::string& outputPath, const PackageReader::CachePair* curPair, const std::string& packageName)
{
	const Entries::FileNode* internalNode = curPair->getFileEntry(internalPath);
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
	const PackageReader::CachePair* pkg = (*m_package)[packageName][PackageReader::PackageTrioType::H];
	if (pkg != nullptr)
	{
		const Entries::FileNode* fileNode = pkg->getFileEntry(internalPath);
		uint64_t timeRaw = fileNode->getTimeStamp();
		FileProperties::writeWinTime(std::filesystem::path(filePath), timeRaw, timeRaw, timeRaw);
	}
}
