#include "BatchIterator.h"

using namespace WarframeExporter;

BatchIterator::BatchIterator(LotusLib::PackageCollection<LotusLib::CachePairReader>* package, const Ensmallening& ensmallData, std::filesystem::path baseOutputPath)
	: m_package(package),
	m_ensmalleningData(ensmallData),
	m_baseOutPath(baseOutputPath),
	m_logger(Logger::getInstance())
{
}

void
BatchIterator::batchIterate(const LotusLib::LotusPath& basePath, const std::vector<std::string>& packages, ExtractorType types)
{
	this->validatePackages(packages);

	for (const std::string& curPackageName : packages)
	{
		auto curPair = (*m_package)[curPackageName][LotusLib::PackageTrioType::H];
		curPair->readToc();
		(*m_package)[curPackageName][LotusLib::PackageTrioType::B]->readToc();
		(*m_package)[curPackageName][LotusLib::PackageTrioType::F]->readToc();

		for (auto iter = curPair->getIter(basePath); iter != curPair->getIter(); iter++)
		{
			const LotusLib::FileEntries::FileNode* curFile = *iter;

			std::unique_ptr<char[]> rawData = curPair->getDataAndDecompress(curFile);
			BinaryReaderBuffered rawDataReader((uint8_t*)rawData.release (), curFile->getLen());
			std::string curFilePath = curFile->getFullPath();

			LotusLib::CommonHeader header;
			if (!tryReadHeader(rawDataReader, header))
			{
				m_logger.debug("Common header error: " + curFilePath);
				continue;
			}

			Extractor* extractor = g_enumMapExtractor[header.type];

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

			processKnownFile(curPackageName, curFilePath, &rawDataReader, header, extractor);
		}

		curPair->unReadToc();
		(*m_package)[curPackageName][LotusLib::PackageTrioType::B]->unReadToc();
		(*m_package)[curPackageName][LotusLib::PackageTrioType::F]->unReadToc();
	}
}

void
BatchIterator::validatePackages(const std::vector<std::string>& packages) const
{
	for (auto& curPkgStr : packages)
	{
		try
		{
			(*m_package)[curPkgStr][LotusLib::PackageTrioType::H];
		}
		catch (std::exception&)
		{
			m_logger.error("Package does not exist: " + curPkgStr);
			throw std::runtime_error("Package does not exist: " + curPkgStr);
		}
	}
}

bool
BatchIterator::tryReadHeader(BinaryReaderBuffered& rawData, LotusLib::CommonHeader& outHeader) const
{
	try
	{
		int headerLen = LotusLib::CommonHeaderReader::readHeader(rawData.getPtr(), outHeader);
		rawData.seek(headerLen, std::ios::beg);
	}
	catch (LotusLib::LotusException&)
	{
		return false;
	}
	return true;
}
