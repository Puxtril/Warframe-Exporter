#include "BatchIterator.h"

using namespace WarframeExporter;

BatchIterator::BatchIterator()
	: m_logger(Logger::getInstance())
{
}

void
BatchIterator::batchIterate(LotusLib::PackagesReader& pkgsDir, const std::filesystem::path& outputPath, const LotusLib::LotusPath& basePath, const std::vector<std::string>& packages, ExtractorType types, LotusLib::Game game)
{
	for (const std::string& curPackageName : packages)
	{
		std::optional<LotusLib::PackageReader> curPkg = pkgsDir.getPackage(curPackageName);
		if (!curPkg)
		{
			m_logger.warn("Package doesn't exist: " + curPackageName);
			continue;
		}
		
		if (!pkgHasDir(*curPkg, basePath))
		{
			m_logger.warn("Folder doesn't exist inside " + curPackageName + ": " + basePath.string());
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
					processUnknownFile(pkgsDir, curPackageName, curEntry, outputPath);
					continue;
				}

				if (((int)extractor->getExtractorType() & (int)types) == 0)
				{
					processSkipFile(pkgsDir, curPackageName, curEntry, extractor);
					continue;
				}

				processKnownFile(pkgsDir, curPackageName, curEntry, extractor, outputPath);
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

std::vector<std::string>
BatchIterator::getPackageNames(WarframeExporter::ExtractorType types, const std::filesystem::path& cacheDirPath)
{
	LotusLib::PackagesReader pkgs(cacheDirPath);
	std::vector<std::string> pkgNames;
	if ((int)types & (int)WarframeExporter::ExtractorType::Texture)
	{
		if (pkgs.getPackage("Texture"))
		{
			pkgNames.push_back("Texture");
		}
		else
		{
			pkgNames.push_back("TextureDx9");
		}

		if (pkgs.getPackage("LightMap"))
			pkgNames.push_back("LightMap");
	}

	if ((int)types & (int)WarframeExporter::ExtractorType::Level)
	{
		pkgNames.push_back("AnimRetarget");
	}

	if ((int)types & (int)WarframeExporter::ExtractorType::Model ||
		(int)types & (int)WarframeExporter::ExtractorType::Material ||
		(int)types & (int)WarframeExporter::ExtractorType::Audio)
	{
		if (pkgs.getPackage("Misc_xx"))
		{
			pkgNames.push_back("Misc");
			pkgNames.push_back("Misc_xx");
		}
	}
	
	if ((int)types & (int)WarframeExporter::ExtractorType::Shader)
	{
		if (pkgs.getPackage("ShaderDx9"))
		{
			pkgNames.push_back("ShaderDx9");
			pkgNames.push_back("ShaderPermutationDx9");
		}

		if (pkgs.getPackage("ShaderDx10"))
		{
			pkgNames.push_back("ShaderDx10");
			pkgNames.push_back("ShaderPermutationDx10");
		}
		
		if (pkgs.getPackage("ShaderDx11"))
		{
			pkgNames.push_back("ShaderDx11");
			pkgNames.push_back("ShaderPermutationDx11");
		}

		if (pkgs.getPackage("ShaderDx12"))
		{
			pkgNames.push_back("ShaderDx12");
			pkgNames.push_back("ShaderPermutationDx12");
		}
	}

	return pkgNames;
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