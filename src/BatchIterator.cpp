#include "BatchIterator.h"

using namespace WarframeExporter;

BatchIterator::BatchIterator(const std::filesystem::path& pkgsDir, const Ensmallening& ensmallData, std::filesystem::path baseOutputPath)
	: m_package(pkgsDir),
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
		LotusLib::PackageReader curPkg = m_package.getPackage(curPackageName);

		for (auto iter = curPkg.getIter(basePath); iter != curPkg.getIter(); iter++)
		{
			try
			{
				LotusLib::FileEntry curEntry = curPkg.getFile((*iter), LotusLib::FileEntryReaderFlags::READ_COMMON_HEADER | LotusLib::FileEntryReaderFlags::READ_H_CACHE);

				Extractor* extractor = g_enumMapExtractor[curEntry.commonHeader.type];

				if (extractor == nullptr)
				{
					processUnknownFile(curPkg, curEntry);
					continue;
				}

				if (((int)extractor->getExtractorType() & (int)types) == 0)
				{
					processSkipFile(curPkg, curEntry, extractor);
					continue;
				}

				processKnownFile(curPkg, curEntry, extractor);
			}
			catch (LotusLib::DecompressionException& ex)
			{
				m_logger.error("Decompression error, skipping");
				continue;
			}
			catch (LotusLib::LotusException& ex)
			{
				continue;
			}
		}
	}
}

void
BatchIterator::validatePackages(const std::vector<std::string>& packages)
{
	for (auto& curPkgStr : packages)
	{
		try
		{
			m_package.getPackage(curPkgStr);
		}
		catch (std::exception&)
		{
			m_logger.error("Package does not exist: " + curPkgStr);
			throw std::runtime_error("Package does not exist: " + curPkgStr);
		}
	}
}