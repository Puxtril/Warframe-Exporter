#include "ExtractFile.h"

int
WarframeExporter::getExtractableCount(const LotusLib::PackageCollection& pkgsDir, const std::string& basePath, ExtractorType types, LotusLib::Game game)
{
	LotusLib::PackageCategory targetPkgCategories = g_enumMapExtractor.getPkgCategories(game, types);

	int extractableCount = 0;
	for (const LotusLib::Package& curPkg : pkgsDir)
	{
		if (((int)curPkg.getPkgCategory() & (int)targetPkgCategories) == 0)
		{
			// Package not needed by specified extractors
			continue;
		}
		
		if (!curPkg.dirExists(LotusLib::PkgSplitType::HEADER, basePath))
		{
			continue;
		}

		for (auto iter = curPkg.getIter(basePath); iter != curPkg.getIter(); iter++)
		{
			try
			{
				uint32_t curEntryType = curPkg.readCommonHeaderFormat(*iter);
				Extractor* extractor = g_enumMapExtractor.at(game, curPkg.getPkgCategory(), curEntryType);
				if (extractor == nullptr || ((int)extractor->getExtractorType() & (int)types) == 0)
					continue;

				extractableCount++;
			}
			catch (LotusLib::CommonHeaderError& ex)
			{
				continue;
			}
		}
	}

	return extractableCount;
}

void
WarframeExporter::extractAllFiles(const LotusLib::PackageCollection& pkgsDir, const LotusLib::PackagesBin& pkgsBin, const std::filesystem::path& outputPath, const std::string& basePath, ExtractorType types, ExtractOptions options)
{
	Logger& logger = WarframeExporter::Logger::getInstance();
    LotusLib::PackageCategory targetPkgCategories = g_enumMapExtractor.getPkgCategories(pkgsDir.getGame(), types);

	for (const LotusLib::Package& curPkg : pkgsDir)
	{
		if (((int)curPkg.getPkgCategory() & (int)targetPkgCategories) == 0)
		{
			// Package not needed by specified extractors
			continue;
		}
		
		if (!curPkg.dirExists(LotusLib::PkgSplitType::HEADER, basePath))
		{
			logger.warn("Folder doesn't exist inside " + curPkg.getName() + ": " + basePath);
			continue;
		}

		for (auto iter = curPkg.getIter(basePath); iter != curPkg.getIter(); iter++)
		{
			try
			{
				uint32_t curEntryType = curPkg.readCommonHeaderFormat(*iter);
				Extractor* extractor = g_enumMapExtractor.at(pkgsDir.getGame(), curPkg.getPkgCategory(), curEntryType);

				if (extractor == nullptr || ((int)extractor->getExtractorType() & (int)types) == 0)
					continue;

                LotusLib::FileEntry fileEntry = curPkg.getFileEntry(*iter);
				extractFile(pkgsDir, pkgsBin, fileEntry, extractor, outputPath, options);
			}
			catch (LotusLib::DecompressionException&)
			{
				logger.error("Decompression error, skipping");
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
WarframeExporter::tryExtractFile(const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, const std::string& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types, WarframeExporter::ExtractOptions options)
{
	LotusLib::PackageCategory pkgCategory = WarframeExporter::g_enumMapExtractor.getPkgCategories(pkgs.getGame(), types);
	for (const LotusLib::Package& curPkg : pkgs)
	{
		if (((int)curPkg.getPkgCategory() & (int)pkgCategory) == 0)
		{
			// Package not needed by specified extractors
			continue;
		}

		try
		{
			LotusLib::FileNode fileNode = curPkg.getFileNode(PkgSplitType::HEADER, intPath);

			uint32_t curEntryType = curPkg.readCommonHeaderFormat(fileNode);
			WarframeExporter::Extractor* extractor = WarframeExporter::g_enumMapExtractor.at(pkgs.getGame(), curPkg.getPkgCategory(), curEntryType);

			if (extractor == nullptr || ((int)extractor->getExtractorType() & (int)types) == 0)
				continue;

			LotusLib::FileEntry fileEntry = curPkg.getFileEntry(fileNode);
			WarframeExporter::extractFile(pkgs, pkgsBin, fileEntry, extractor, outPath, options);
			
			return true;
		}
		catch (std::exception&)
		{
			continue;
		}
	}
	
	return false;
}

void
WarframeExporter::extractFile(const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin, LotusLib::FileEntry& fileEntry, Extractor* extractor, const std::filesystem::path& outputPath, const ExtractOptions options)
{
	Logger& logger = WarframeExporter::Logger::getInstance();
	const std::string internalPath = LotusLib::getFullPath(fileEntry.headerNode);

    std::filesystem::path savePath;
	if (extractor->isMultiExport())
	{
		savePath = outputPath / std::filesystem::path(internalPath).relative_path();
		savePath.replace_extension("");

		if (!std::filesystem::exists(savePath) && !options.dryRun)
			std::filesystem::create_directories(savePath);
	}
	else
	{
		savePath = outputPath / std::filesystem::path(internalPath).relative_path();
		savePath.replace_extension(extractor->getOutputExtension(fileEntry.commonHeader, &fileEntry.header, options));

		if (existingFileIdentical(fileEntry.headerNode.timeStamp, savePath))
		{
			logger.info("Identical file time, skipping: " + internalPath);
			return;
		}

		if (!std::filesystem::exists(savePath.parent_path()) && !options.dryRun)
			std::filesystem::create_directories(savePath.parent_path());
	}

	try
	{		
		logger.info("Extracting " + internalPath);
		logger.debug(spdlog::fmt_lib::format("Extracting as {}, Enum={}", extractor->getFriendlyName(), fileEntry.commonHeader.type));

		extractor->extract(fileEntry, pkgs, pkgsBin, savePath, options);
		writeFileProperties(fileEntry.headerNode.timeStamp, savePath);
	}
	catch (not_imeplemented_error& err)
	{
		logger.warn("Not implemented: " + std::string(err.what()) + " " + internalPath);
	}
	catch (unknown_format_error& err)
	{
		logger.error("Unknown Format: " + std::string(err.what()) + " " + internalPath);
	}
	catch (std::exception& err)
	{
		logger.error(std::string(err.what()) + ": " + internalPath);
	}
}

bool
WarframeExporter::existingFileIdentical(int64_t timestamp, const std::filesystem::path& filePath)
{
	if (!std::filesystem::exists(filePath))
		return false;

	FileProperties::TimeDos existingTime = FileProperties::readDos(filePath, true);

	if (FileProperties::wipeNanoseconds(timestamp) != existingTime)
		return false;

	return true;
}

void
WarframeExporter::writeFileProperties(int64_t timestamp, const std::filesystem::path filePath)
{
	FileProperties::writeDos(std::filesystem::path(filePath), timestamp, timestamp, timestamp);
}
