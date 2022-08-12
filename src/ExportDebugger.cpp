#include "ExportDebugger.h"
#include <filesystem>

using namespace WarframeExporter;

ExportDebugger::ExportDebugger(PackageReader::PackageDir* package, const Ensmallening& ensmallData, std::string baseOutputPath)
	: m_package(package),
	m_pathManager(baseOutputPath),
	m_ensmalleningData(ensmallData),
	m_enumMap(m_enumMap.getInstance())
{
}

void
ExportDebugger::debugBatchExtract(std::string internalBasePath, std::vector<std::string> packages, FileTypeInternal types)
{
	Logger loggerDebug = Logger(m_pathManager.getLogDebugFilePath(), true);
	packages = this->validatePackages(internalBasePath, packages, loggerDebug);

	for (const std::string& curPackageName : packages)
	{
		const PackageReader::CachePair* curPair = (*m_package)[curPackageName][PackageReader::PackageTrioType::H];
		for (auto start = curPair->getIteratorRecursive(internalBasePath), end = curPair->end(); start != end; ++start)
		{
			const Entries::FileNode* curFile = *start;
			std::string fullFilePath = curFile->getFullPath();
			BinaryReaderBuffered* rawData = new BinaryReaderBuffered(curPair->getDataAndDecompress(curFile), curFile->getLen());
			
			CommonFileHeader* header = nullptr;
			try 
			{
				header = new CommonFileHeader(*rawData);
			}
			catch (LimitException&)
			{
				delete header;
				continue;
			}

			try
			{
				Extractor& extractor = m_enumMap.getExtractor(static_cast<FileTypeExternal>(header->getEnum()));
				if (((int)extractor.internalType() & (int)types) == 0)
					continue;
				debugExtract(
					fullFilePath,
					curPackageName,
					rawData,
					extractor,
					*header,
					loggerDebug
				);
				delete header;
				delete rawData;	
			}
			catch (std::out_of_range&)
			{
				delete header;
				delete rawData;
				continue;
			}
		}
	}
}

void
ExportDebugger::debugExtract(const std::string internalPath, const std::string& pkgName, BinaryReaderBuffered* hReader, Extractor& extractor, const CommonFileHeader& header, Logger& logDebug)
{
	BinaryReaderBuffered* bReader = nullptr;
	BinaryReaderBuffered* fReader = nullptr;
	try
	{
		bReader = getBodyReader(internalPath, pkgName, PackageReader::PackageTrioType::B);
		fReader = getBodyReader(internalPath, pkgName, PackageReader::PackageTrioType::F);
		extractor.extractDebug(header, hReader, bReader, fReader, m_ensmalleningData);
	}
	catch (not_imeplemented_error& err)
	{
		std::string privateMsg = std::string(err.what()) + ": " + internalPath;
		logDebug.log(privateMsg, err.getDebugCount(), err.getDebugs());
		std::cout << privateMsg << std::endl;
		writeAllDebugs(internalPath, hReader, bReader, fReader);
	}
	catch (unknown_format_error& err)
	{
		std::string privateMsg = std::string(err.what()) + ": " + internalPath;
		logDebug.log(privateMsg, err.getDebugCount(), err.getDebugs());
		std::cout << privateMsg << std::endl;
		writeAllDebugs(internalPath, hReader, bReader, fReader);
	}
	catch (std::runtime_error& err)
	{
		std::string privateMsg = std::string(err.what()) + ": " + internalPath;
		logDebug.log(privateMsg);
		std::cout << privateMsg << std::endl;
		writeAllDebugs(internalPath, hReader, bReader, fReader);
	}
	delete bReader;
	delete fReader;
}

void
ExportDebugger::printEnumCounts(const std::string& package)
{
	std::map<uint32_t, int> enumCounts;
	
	const PackageReader::CachePair* curPair = (*m_package)[package][PackageReader::PackageTrioType::H];
	for (const auto& curFile : *curPair)
	{
		try
		{
			BinaryReaderBuffered rawData(curPair->getDataAndDecompress(curFile), curFile->getLen());
			CommonFileHeader header(rawData);
			enumCounts[header.getEnum()]++;
		}
		catch (DecompressionException&)
		{
			std::cout << "Decompress error: " << curFile->getFullPath() << std::endl;
			continue;
		}
		catch (LimitException&)
		{
			//std::cout << "Common Header Error [" << ex.what() << "] " << curFile->getFullPath() << std::endl;
			continue;
		}
	}

	for (auto& x : enumCounts)
	{
		std::cout << x.first << ": " << x.second << std::endl;
		printEnumExamples(package, x.first, 5);
	}
}

void
ExportDebugger::printEnumExamples(const std::string& package, uint32_t enumNum, int printCount)
{
	const PackageReader::CachePair* curPair = (*m_package)[package][PackageReader::PackageTrioType::H];
	int curPrintCount = 0;
	int curFoundCount = 0;
	for (const auto& curFile : *curPair)
	{
		BinaryReaderBuffered rawData(curPair->getDataAndDecompress(curFile), curFile->getLen());
		try
		{
			CommonFileHeader header(rawData);
		
			if (header.getEnum() == enumNum)
			{
				if (curFoundCount % 5 != 0)
				{
					std::cout << "  " << curFile->getFullPath() << std::endl;
					curPrintCount++;
				}
				curFoundCount++;
			}

			if (curPrintCount > printCount)
				break;
		}
		catch (LimitException&)
		{
			continue;
		}
	}
}

BinaryReaderBuffered*
ExportDebugger::getBodyReader(const std::string& internalPath, const std::string& packageName, PackageReader::PackageTrioType type)
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

std::vector<std::string>
ExportDebugger::validatePackages(std::string internalBasePath, std::vector<std::string> packages, Logger& logger)
{
	std::vector<std::string> validPackages;
	for (auto& curPkgStr : packages)
	{
		const Entries::DirNode* foundDir = (*m_package)[curPkgStr][PackageReader::PackageTrioType::H]->getDirEntry(internalBasePath);
		if (foundDir != nullptr)
			validPackages.push_back(curPkgStr);
		else
			logger.log("Does not exist in " + curPkgStr + " :" + internalBasePath);
	}
	return validPackages;
}

void
ExportDebugger::writeAllDebugs(const std::string& internalPath, BinaryReaderBuffered* HReader, BinaryReaderBuffered* BReader, BinaryReaderBuffered* FReader)
{
	if (HReader != nullptr)
	{
		std::string debugHPath = m_pathManager.getDebugOutputFilePath(internalPath, "_H");
		std::ofstream out;
		out.open(debugHPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		HReader->seek(0, std::ios_base::beg);
		out.write(HReader->getPtr(), HReader->getLength());
		out.close();
	}
	if (BReader != nullptr)
	{
		std::string debugBPath = m_pathManager.getDebugOutputFilePath(internalPath, "_B");
		std::ofstream out;
		out.open(debugBPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		BReader->seek(0, std::ios_base::beg);
		out.write(BReader->getPtr(), BReader->getLength());
		out.close();
	}
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
