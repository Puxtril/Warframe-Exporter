#include "ExportDebugger.h"

using namespace WarframeExporter;

ExportDebugger::ExportDebugger(PackageReader::PackageDir* package, const Ensmallening& ensmallData, std::string baseOutputPath)
	: m_package(package),
	m_pathManager(baseOutputPath),
	m_ensmalleningData(ensmallData),
	m_logger(Logger::getInstance())
{
}

void
ExportDebugger::debugBatchExtract(std::string internalBasePath, std::vector<std::string> packages, ExtractorType types)
{
	this->validatePackages(internalBasePath, packages);
	PackageDirLimited pkgParam = PackageDirLimited(m_package);

	for (const std::string& curPackageName : packages)
	{
		const PackageReader::CachePair* curPair = (*m_package)[curPackageName][PackageReader::PackageTrioType::H];
		for (auto start = curPair->getIteratorRecursive(internalBasePath), end = curPair->end(); start != end; ++start)
		{
			const Entries::FileNode* curFile = *start;
			std::string fullFilePath = curFile->getFullPath();
			BinaryReaderBuffered rawData(curPair->getDataAndDecompress(curFile), curFile->getLen());
			
			CommonFileHeader header;
			if (!tryReadHeader(rawData, header))
			{
				m_logger.debug("Common header error: " + curFile->getFullPath());
				continue;
			}

			Extractor* extractor = g_enumMapExtractor[header.getEnum()];
			if (((int)extractor->getExtractorType() & (int)types) == 0)
				continue;
				
			debugExtract(pkgParam, curPackageName, curFile->getFullPath(), &rawData, header, extractor);
		}
	}
}

void
ExportDebugger::debugExtract(PackageDirLimited& pkgParam, const std::string& packageName, const std::string internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, Extractor* extractor)
{
	try
	{
		extractor->extractDebug(header, hReader, pkgParam, packageName, internalPath, m_ensmalleningData);
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
ExportDebugger::printEnumCounts(const std::string& package)
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
ExportDebugger::validatePackages(std::string internalBasePath, std::vector<std::string> packages)
{
	for (auto& curPkgStr : packages)
	{
		try
		{
			(*m_package)[curPkgStr][PackageReader::PackageTrioType::H];
		}
		catch (std::exception&)
		{
			m_logger.error("Package does not exist: " + curPkgStr);
			throw std::runtime_error("Package does not exist: " + curPkgStr);
		}
	}
}

void
ExportDebugger::writeAllDebugs(PackageDirLimited& pkgParam, const std::string& packageName, const std::string internalPath)
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

bool
ExportDebugger::tryReadHeader(BinaryReaderBuffered& rawData, CommonFileHeader& outHeader)
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
