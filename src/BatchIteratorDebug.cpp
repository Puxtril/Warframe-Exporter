#include "BatchIteratorDebug.h"

using namespace WarframeExporter;

BatchIteratorDebug::BatchIteratorDebug(PackageReader::PackageDir* package, const Ensmallening& ensmallData, std::string baseOutputPath)
	: BatchIterator(package, ensmallData, baseOutputPath)
{
}

void
BatchIteratorDebug::processKnownFile(PackageDirLimited& pkgParam, const std::string& packageName, const std::string& internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, Extractor* extractor)
{
	try
	{
		extractor->extractDebug(header, hReader, pkgParam, packageName, internalPath, m_ensmalleningData);
		m_logger.debug("Successfully processed: " + internalPath);
	}
	catch (not_imeplemented_error& err)
	{
		m_logger.debug("Not implemented: " + std::string(err.what()) + " " + internalPath);
		writeAllDebugs(pkgParam, packageName, internalPath);
	}
	catch (unknown_format_error& err)
	{
		m_logger.debug("Unknown Format: " + std::string(err.what()) + " " + internalPath);
		writeAllDebugs(pkgParam, packageName, internalPath);
	}
	catch (std::runtime_error& err)
	{
		m_logger.error(std::string(err.what()) + ": " + internalPath);
		writeAllDebugs(pkgParam, packageName, internalPath);
	}
}

void
BatchIteratorDebug::processUnknownFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file)
{
	m_logger.debug("Unknown file type " + std::to_string(header.getEnum()) + ": " + internalPath);
}

void
BatchIteratorDebug::processSkipFile(const std::string& internalPath, const CommonFileHeader& header, const Entries::FileNode* file, const Extractor* extractor)
{
	m_logger.debug("Skipping file type " + std::to_string(header.getEnum()) + " (" + extractor->getFriendlyName() + "): " + internalPath);
}

void
BatchIteratorDebug::printEnumCounts(const std::string& package)
{
	std::map<uint32_t, int> enumCounts;
	std::map<uint32_t, std::vector<std::string>> enumExamples;

	const PackageReader::CachePair* curPair = (*m_package)[package][PackageReader::PackageTrioType::H];
	for (const auto& curFile : *curPair)
	{
		try
		{
			BinaryReaderBuffered rawData(curPair->getDataAndDecompress(curFile), curFile->getLen());
			CommonFileHeader header(rawData);
			enumCounts[header.getEnum()]++;
			
			if (enumExamples[header.getEnum()].size() < 10)
				enumExamples[header.getEnum()].push_back(curFile->getFullPath());
			
			// Every 10 new files, replace an existing file
			else if (enumCounts[header.getEnum()] % 10 == 0)
			{
				int newIndex = enumCounts[header.getEnum()] % 100 / 10;
				enumExamples[header.getEnum()][newIndex] = curFile->getFullPath();
			}
		}
		catch (DecompressionException&)
		{
			m_logger.warn("Decompress error: " + curFile->getFullPath());
			continue;
		}
		catch (LimitException& ex)
		{
			m_logger.warn(std::string(ex.what()) + curFile->getFullPath());
			continue;
		}
		catch (std::exception& ex)
		{
			m_logger.error(std::string(ex.what()) + ": " + curFile->getFullPath());
			continue;
		}
	}

	for (auto& x : enumCounts)
	{
		std::cout << x.first << ": " << x.second << std::endl;
		for (auto& str : enumExamples[x.first])
			std::cout << "  " << str << std::endl;
	}
}

void
BatchIteratorDebug::writeAllDebugs(PackageDirLimited& pkgParam, const std::string& packageName, const std::string internalPath)
{
	BinaryReaderBuffered* HReader = pkgParam.getFileReader(packageName, PackageReader::PackageTrioType::H, internalPath);
	if (HReader != nullptr)
	{
		std::string debugHPath = m_pathManager.getDebugOutputFilePath(internalPath, "_H");
		std::ofstream out;
		out.open(debugHPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		HReader->seek(0, std::ios_base::beg);
		out.write(HReader->getPtr(), HReader->getLength());
		out.close();
	}
	BinaryReaderBuffered* BReader = pkgParam.getFileReader(packageName, PackageReader::PackageTrioType::B, internalPath);
	if (BReader != nullptr)
	{
		std::string debugBPath = m_pathManager.getDebugOutputFilePath(internalPath, "_B");
		std::ofstream out;
		out.open(debugBPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		BReader->seek(0, std::ios_base::beg);
		out.write(BReader->getPtr(), BReader->getLength());
		out.close();
	}
	BinaryReaderBuffered* FReader = pkgParam.getFileReader(packageName, PackageReader::PackageTrioType::F, internalPath);
	if (FReader != nullptr)
	{
		std::string debugFPath = m_pathManager.getDebugOutputFilePath(internalPath, "_F");
		std::ofstream out;
		out.open(debugFPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		FReader->seek(0, std::ios_base::beg);
		out.write(FReader->getPtr(), FReader->getLength());
		out.close();
	}
}