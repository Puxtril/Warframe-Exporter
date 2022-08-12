#include "BatchExtractor.h"
#include <filesystem>

using namespace WarframeExporter;

BatchExtractor::BatchExtractor(PackageReader::PackageDir* package, const Ensmallening& ensmalleningData, std::string baseOutputPath)
	: m_package(package),
	m_ensmalleningData(ensmalleningData),
	m_pathManager(baseOutputPath),
	m_enumMap(m_enumMap.getInstance())
{
}

void
BatchExtractor::batchExtract(std::string basePath, std::vector<std::string> packages, FileTypeInternal types, bool overwriteLogs, bool includePrivateLog)
{
	Logger logPrivate = Logger("", true);
	if (includePrivateLog)
		logPrivate = Logger(m_pathManager.getLogPrivateFilePath(), overwriteLogs);
	Logger logPublic = Logger(m_pathManager.getLogPublicFilePath(), overwriteLogs);

	this->validatePackages(packages, logPrivate);
	for (const std::string& curPackageName : packages)
	{
		const PackageReader::CachePair* curPair = (*m_package)[curPackageName][PackageReader::PackageTrioType::H];
		for (auto start = curPair->getIteratorRecursive(basePath), end = curPair->end(); start != end; ++start)
		{
			const Entries::FileNode* curFile = *start;
			BinaryReaderBuffered rawData(curPair->getDataAndDecompress(curFile), curFile->getLen());
			
			CommonFileHeader header;
			if (!tryReadHeader(rawData, header))
				continue;

			try
			{
				Extractor& extractor = m_enumMap.getExtractor(static_cast<FileTypeExternal>(header.getEnum()));
				if (((int)extractor.internalType() & (int)types) == 0)
					continue;

				std::string outputPath = m_pathManager.getOutputFilePath(curFile->getFullPath(), extractor.outFileExt());
				if (existingFileIdentical(curFile->getFullPath(), outputPath, curPair, curPackageName))
					continue;

				extractOrLog(
					curFile->getFullPath(),
					curPackageName,
					outputPath,
					&rawData,
					extractor,
					header,
					logPrivate,
					logPublic
				);
			}
			catch (std::out_of_range&)
			{
				continue;
			}
		}
	}
}

void
BatchExtractor::extractOrLog(
	const std::string internalPath,
	const std::string& packageName,
	const std::string& outputPath,
	BinaryReaderBuffered* hReader,
	Extractor& extractor,
	const CommonFileHeader& header,
	Logger& logPrivate,
	Logger& logPublic
)
{
	BinaryReaderBuffered* bReader = nullptr;
	BinaryReaderBuffered* fReader = nullptr;
	try
	{
		bReader = getBodyReader(internalPath, packageName, PackageReader::PackageTrioType::B);
		fReader = getBodyReader(internalPath, packageName, PackageReader::PackageTrioType::F);
		
		extractor.extract(header, hReader, bReader, fReader, m_ensmalleningData, outputPath);
		writeFileProperties(outputPath, internalPath, packageName);
		std::cout << internalPath << std::endl;
	}
	catch (not_imeplemented_error& err)
	{
		std::string publicMsg = "Operation not implemented: " + internalPath;
		std::string privateMsg = std::string(err.what()) + ": " + internalPath;

		logPrivate.log(privateMsg, err.getDebugCount(), err.getDebugs());
		logPublic.log(publicMsg);
		std::cout << privateMsg << std::endl;
	}
	catch (unknown_format_error& err)
	{
		std::string publicMsg = "Unknown Format: " + internalPath;
		std::string privateMsg = std::string(err.what()) + ": " + internalPath;

		logPrivate.log(privateMsg, err.getDebugCount(), err.getDebugs());
		logPublic.log(publicMsg);
		std::cout << privateMsg << std::endl;
	}
	catch (std::exception& err)
	{
		std::string publicMsg = "Unknown Error: " + internalPath;
		std::string privateMsg = std::string(err.what()) + ": " + internalPath;

		logPrivate.log(privateMsg);
		logPublic.log(publicMsg);
		std::cout << privateMsg << std::endl;
	}
	delete bReader;
	delete fReader;
}

bool
BatchExtractor::existingFileIdentical(const std::string& internalPath, const std::string& outputPath, const PackageReader::CachePair* curPair, const std::string& packageName)
{
	const Entries::FileNode* internalNode = curPair->getFileEntry(internalPath);
	std::filesystem::path p(outputPath);
	
	if (!std::filesystem::exists(p))
		return false;

	int64_t existingTime = FileProperties::readWinTimeMod(p);
	if (internalNode->getTimeStamp() != existingTime)
		return false;

	return true;
}

void
BatchExtractor::validatePackages(std::vector<std::string> packages, Logger& logger)
{
	for (auto& curPkgStr : packages)
	{
		try
		{
			const PackageReader::CachePair* pair = (*m_package)[curPkgStr][PackageReader::PackageTrioType::H];
		}
		catch (std::exception&)
		{
			logger.log("Package does not exist: " + curPkgStr);
			throw std::runtime_error("Package does not exist: " + curPkgStr);
		}
	}
}

BinaryReaderBuffered*
BatchExtractor::getBodyReader(const std::string& internalPath, const std::string& packageName, PackageReader::PackageTrioType type)
{
	const PackageReader::CachePair* pkg = (*m_package)[packageName][type];
	if (pkg != nullptr)
	{
		try {
			const Entries::FileNode* fileNode = pkg->getFileEntry(internalPath);
			BinaryReaderBuffered* reader = new BinaryReaderBuffered(pkg->getDataAndDecompress(fileNode), fileNode->getLen());
			return reader;
		} catch (std::filesystem::filesystem_error&) {
			BinaryReaderBuffered* reader = new BinaryReaderBuffered();
			return reader;
		}
	}
	BinaryReaderBuffered* reader = new BinaryReaderBuffered();
	return reader;
}

bool
BatchExtractor::tryReadHeader(BinaryReaderBuffered& rawData, CommonFileHeader& outHeader)
{
	try
	{
		outHeader = CommonFileHeader(rawData);
	}
	catch (LimitException&)
	{
		return false;
	}
	return true;
}

void
BatchExtractor::writeFileProperties(const std::string filePath, const std::string internalPath, const std::string& packageName)
{
	const PackageReader::CachePair* pkg = (*m_package)[packageName][PackageReader::PackageTrioType::H];
	if (pkg != nullptr)
	{
		const Entries::FileNode* fileNode = pkg->getFileEntry(internalPath);
		uint64_t timeRaw = fileNode->getTimeStamp();
		FileProperties::writeWinTime(std::filesystem::path(filePath), timeRaw, timeRaw, timeRaw);
	}
}
