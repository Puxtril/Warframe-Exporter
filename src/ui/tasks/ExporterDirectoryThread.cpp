#include "ui/tasks/ExporterDirectoryThread.h"

ExporterDirectoryThread::ExporterDirectoryThread()
    : QThread(), m_fileCountExtracted(0), m_extractCancelled(false)
{
}

void
ExporterDirectoryThread::setData(LotusLib::PackageCollection* pkgsReader, LotusLib::PackagesBin* pkgsBin, std::filesystem::path exportPath, WarframeExporter::ExtractorType extractTypes, WarframeExporter::ExtractOptions options)
{
    m_pkgsReader = pkgsReader;
	m_pkgsBin = pkgsBin;
    m_exportPath = exportPath;
    m_extractTypes = extractTypes;
    m_options = options;
}

void
ExporterDirectoryThread::setInternalPath(std::string internalPath)
{
    m_internalPath = internalPath;
}

void
ExporterDirectoryThread::run()
{
    emit ExporterDirectoryThread::extractIndexingStarted();
    int totalItems = WarframeExporter::getExtractableCount(*m_pkgsReader, m_internalPath, m_extractTypes, m_pkgsReader->getGame());
    emit ExporterDirectoryThread::extractStart(totalItems);

    try
    {
        extractAllFiles(*m_pkgsReader, m_exportPath, m_internalPath, m_extractTypes, m_options);
    }
    catch (ExtractionCancelled&)
    {
        return;
    }
    catch (std::exception& ex)
    {
        emit ExporterDirectoryThread::extractError(ex.what());
        return;
    }

    emit ExporterDirectoryThread::extractComplete(m_fileCountExtracted);
}

void
ExporterDirectoryThread::extractAllFiles(LotusLib::PackageCollection& pkgsDir, const std::filesystem::path& outputPath, const std::string& basePath, WarframeExporter::ExtractorType types, WarframeExporter::ExtractOptions options)
{
	WarframeExporter::Logger& logger = WarframeExporter::Logger::getInstance();
    LotusLib::PackageCategory targetPkgCategories = WarframeExporter::g_enumMapExtractor.getPkgCategories(pkgsDir.getGame(), types);

	m_fileCountExtracted = 0;
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
				WarframeExporter::Extractor* extractor = WarframeExporter::g_enumMapExtractor.at(pkgsDir.getGame(), curPkg.getPkgCategory(), curEntryType);

				if (extractor == nullptr || ((int)extractor->getExtractorType() & (int)types) == 0)
					continue;

                if (m_extractCancelled)
                {
                    m_extractCancelled = false;
                    throw ExtractionCancelled();
                }
                LotusLib::FileEntry fileEntry = curPkg.getFileEntry(*iter);
                WarframeExporter::extractFile(pkgsDir, *m_pkgsBin, fileEntry, extractor, outputPath, options);
                emit extractItemComplete(++m_fileCountExtracted);
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

void
ExporterDirectoryThread::extractCancelled()
{
    m_extractCancelled = true;
}