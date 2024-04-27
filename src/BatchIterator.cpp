#include "BatchIterator.h"

using namespace WarframeExporter;

BatchIterator::BatchIterator()
	: m_logger(Logger::getInstance())
{
}

void
BatchIterator::batchIterate(LotusLib::PackagesReader& pkgsDir, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath, const LotusLib::LotusPath& basePath, const std::vector<std::string>& packages, ExtractorType types)
{
	this->validatePackages(pkgsDir, packages);

	for (const std::string& curPackageName : packages)
	{
		LotusLib::PackageReader curPkg = pkgsDir.getPackage(curPackageName);
		
		if (!pkgHasDir(curPkg, basePath))
			continue;

		for (auto iter = curPkg.getIter(basePath); iter != curPkg.getIter(); iter++)
		{
			try
			{
				LotusLib::FileEntry curEntry = curPkg.getFile((*iter), LotusLib::FileEntryReaderFlags::READ_COMMON_HEADER | LotusLib::FileEntryReaderFlags::READ_H_CACHE);

				Extractor* extractor = g_enumMapExtractor[curEntry.commonHeader.type];

				if (extractor == nullptr)
				{
					processUnknownFile(pkgsDir, curPackageName, curEntry, ensmalleningData, outputPath);
					continue;
				}

				if (((int)extractor->getExtractorType() & (int)types) == 0)
				{
					processSkipFile(pkgsDir, curPackageName, curEntry, extractor);
					continue;
				}

				processKnownFile(pkgsDir, curPackageName, curEntry, extractor, ensmalleningData, outputPath);
			}
			catch (LotusLib::DecompressionException&)
			{
				m_logger.error("Decompression error, skipping");
				continue;
			}
			catch (LotusLib::LotusException&)
			{
				continue;
			}
		}
	}
}

void
BatchIterator::validatePackages(LotusLib::PackagesReader& pkgsDir, const std::vector<std::string>& packages)
{
	for (auto& curPkgStr : packages)
	{
		try
		{
			pkgsDir.getPackage(curPkgStr);
		}
		catch (std::exception&)
		{
			m_logger.error("Package does not exist: " + curPkgStr);
			throw std::runtime_error("Package does not exist: " + curPkgStr);
		}
	}
}

bool
BatchIterator::pkgHasDir(LotusLib::PackageReader& pkg, const LotusLib::LotusPath& path)
{
	try
	{
		pkg.getDirMeta(path);
		return true;
	}
	catch (std::exception&)
	{
		return false;
	}
}