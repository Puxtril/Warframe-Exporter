#include "BatchIterator.h"

using namespace WarframeExporter;

BatchIterator::BatchIterator()
	: m_logger(Logger::getInstance())
{
}

void
BatchIterator::batchIterate(LotusLib::PackagesReader& pkgsDir, const std::filesystem::path& outputPath, const LotusLib::LotusPath& basePath, ExtractorType types, LotusLib::Game game, ExtractOptions options)
{
	LotusLib::PackageCategory targetPkgCategories = g_enumMapExtractor.getPkgCategories(game, types);
	batchIterate(pkgsDir, outputPath, basePath, targetPkgCategories, types, game, options);
}

void
BatchIterator::batchIterate(LotusLib::PackagesReader& pkgsDir, const std::filesystem::path& outputPath, const LotusLib::LotusPath& basePath, LotusLib::PackageCategory pkgCategory, ExtractorType types, LotusLib::Game game, ExtractOptions options)
{
	for (std::string& curPkgName : pkgsDir)
	{
		std::optional<LotusLib::PackageReader> curPkg = pkgsDir.getPackage(curPkgName);

		if (!curPkg)
		{
			m_logger.warn("Package doesn't exist: " + curPkgName);
			continue;
		}

		if (((int)curPkg->getPkgCategory() & (int)pkgCategory) == 0)
		{
			// Package not needed by specified extractors
			continue;
		}
		
		if (!pkgHasDir(*curPkg, basePath))
		{
			m_logger.warn("Folder doesn't exist inside " + curPkgName + ": " + basePath.string());
			continue;
		}

		for (auto iter = curPkg->getIter(basePath); iter != curPkg->getIter(); iter++)
		{
			try
			{
				LotusLib::FileEntry curEntry = curPkg->getFile((*iter), LotusLib::FileEntryReaderFlags::READ_COMMON_HEADER | LotusLib::FileEntryReaderFlags::READ_H_CACHE);

				Extractor* extractor = g_enumMapExtractor.at(game, curPkg->getPkgCategory(), curEntry.commonHeader.type);

				if (extractor == nullptr)
				{
					processUnknownFile(pkgsDir, curEntry, outputPath);
					continue;
				}

				if (((int)extractor->getExtractorType() & (int)types) == 0)
				{
					processSkipFile(pkgsDir, curEntry, extractor);
					continue;
				}

				processKnownFile(pkgsDir, curEntry, extractor, outputPath, options);
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

bool
BatchIterator::pkgHasDir(LotusLib::PackageReader& pkg, const LotusLib::LotusPath& path)
{
	try
	{
		pkg.getDirNode(path);
		return true;
	}
	catch (std::exception&)
	{
		return false;
	}
}