#include "BatchIterator.h"

using namespace WarframeExporter;

BatchIterator::BatchIterator(PackageReader::PackageDir* package, const Ensmallening& ensmallData, std::string baseOutputPath)
	: m_package(package),
	m_pathManager(baseOutputPath),
	m_ensmalleningData(ensmallData),
	m_logger(Logger::getInstance())
{
}

void
BatchIterator::batchIterate(std::string basePath, std::vector<std::string> packages, ExtractorType types)
{
	this->validatePackages(packages);
	PackageDirLimited pkgParam = PackageDirLimited(m_package);

	for (const std::string& curPackageName : packages)
	{
		const PackageReader::CachePair* curPair = (*m_package)[curPackageName][PackageReader::PackageTrioType::H];

		for (auto start = curPair->getIteratorRecursive(basePath), end = curPair->end(); start != end; ++start)
		{
			const Entries::FileNode* curFile = *start;
			BinaryReaderBuffered rawData(curPair->getDataAndDecompress(curFile), curFile->getLen());
			std::string curFilePath = curFile->getFullPath();

			CommonFileHeader header;
			if (!tryReadHeader(rawData, header))
			{
				m_logger.debug("Common header error: " + curFilePath);
				continue;
			}

			Extractor* extractor = g_enumMapExtractor[header.getEnum()];

			if (extractor == nullptr)
			{
				processUnknownFile(curFilePath, header, curFile);
				continue;
			}

			if (((int)extractor->getExtractorType() & (int)types) == 0)
			{
				processSkipFile(curFilePath, header, curFile, extractor);
				continue;
			}

			processKnownFile(pkgParam, curPackageName, curFilePath, &rawData, header, extractor);
		}
	}
}

void
BatchIterator::validatePackages(std::vector<std::string> packages)
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

bool
BatchIterator::tryReadHeader(BinaryReaderBuffered& rawData, CommonFileHeader& outHeader)
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
