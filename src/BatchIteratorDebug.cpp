#include "BatchIteratorDebug.h"
#include "FileNode.h"

using namespace WarframeExporter;

BatchIteratorDebug::BatchIteratorDebug(LotusLib::PackageCollection<LotusLib::CachePairReader>* package, const Ensmallening& ensmallData, std::filesystem::path baseOutputPath)
	: BatchIterator(package, ensmallData, baseOutputPath), m_outDebugPath(m_baseOutPath / "Debug")
{
}

void
BatchIteratorDebug::processKnownFile(const std::string& packageName, const LotusLib::LotusPath& internalPath, BinaryReaderBuffered* hReader, const LotusLib::CommonHeader& header, Extractor* extractor)
{
	try
	{
		extractor->extractDebug(header, hReader, *m_package, packageName, internalPath, m_ensmalleningData);
		m_logger.debug("Successfully processed: " + internalPath.string());
	}
	catch (not_imeplemented_error& err)
	{
		m_logger.debug("Not implemented: " + std::string(err.what()) + " " + internalPath.string());
		writeAllDebugs(packageName, internalPath);
	}
	catch (unknown_format_error& err)
	{
		m_logger.debug("Unknown Format: " + std::string(err.what()) + " " + internalPath.string());
		writeAllDebugs(packageName, internalPath);
	}
	catch (std::runtime_error& err)
	{
		m_logger.error(std::string(err.what()) + ": " + internalPath.string());
		writeAllDebugs(packageName, internalPath);
	}
}

void
BatchIteratorDebug::processUnknownFile(const LotusLib::LotusPath& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file)
{
	m_logger.debug("Unknown file type " + std::to_string(header.type) + ": " + internalPath.string());
}

void
BatchIteratorDebug::processSkipFile(const LotusLib::LotusPath& internalPath, const LotusLib::CommonHeader& header, const LotusLib::FileEntries::FileNode* file, const Extractor* extractor)
{
	m_logger.debug("Skipping file type " + std::to_string(header.type) + " (" + extractor->getFriendlyName() + "): " + internalPath.string());
}

void
BatchIteratorDebug::printEnumCounts(const std::string& package, const LotusLib::LotusPath& internalPath)
{
	std::map<uint32_t, int> enumCounts;
	std::map<uint32_t, std::vector<std::string>> enumExamples;

	std::shared_ptr<LotusLib::CachePairReader> curPair = (*m_package)[package][LotusLib::PackageTrioType::H];
	curPair->readToc();
	m_logger.info("Collecting Common Header Format stats");
	for (auto iter = curPair->getIter(internalPath); iter != curPair->getIter(); iter++)
	{
		const LotusLib::FileEntries::FileNode* curFile = *iter;
		try
		{
			std::unique_ptr<char[]> rawData = curPair->getDataAndDecompress(curFile);
			BinaryReaderBuffered rawDataReader((uint8_t*)rawData.release(), curFile->getLen());
			
			LotusLib::CommonHeader header;
			if (!tryReadHeader(rawDataReader, header))
				continue;

			enumCounts[header.type]++;
			
			// For the first 10 files, add directly into examples
			if (enumExamples[header.type].size() < 10)
				enumExamples[header.type].push_back(curFile->getFullPath());
			
			// Every 10 new files, replace an existing example
			else if (enumCounts[header.type] % 10 == 0)
			{
				int newIndex = enumCounts[header.type] % 100 / 10;
				enumExamples[header.type][newIndex] = curFile->getFullPath();
			}
		}
		catch (LotusLib::DecompressionException&)
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
	curPair->unReadToc();

	for (auto& x : enumCounts)
	{
		std::cout << x.first << ": " << x.second << std::endl;
		for (auto& str : enumExamples[x.first])
			std::cout << "  " << str << std::endl;
	}
}

void
BatchIteratorDebug::writeAllDebugs(const std::string& packageName, const LotusLib::LotusPath& internalPath)
{
	const LotusLib::FileEntries::FileNode* hFileEntry = (*m_package)[packageName][LotusLib::PackageTrioType::H]->getFileEntry(internalPath);
	std::unique_ptr<char[]> hDataRaw = (*m_package)[packageName][LotusLib::PackageTrioType::H]->getDataAndDecompress(hFileEntry);
	
	std::filesystem::path debugPath = m_outDebugPath / internalPath.getPreferredPath().relative_path();
	if (!std::filesystem::exists(debugPath.parent_path()))
		std::filesystem::create_directories(debugPath.parent_path());

	if (hDataRaw)
	{
		debugPath.replace_filename(internalPath.stem().string() + "_H");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write(hDataRaw.get(), hFileEntry->getLen());
		out.close();
	}

	const LotusLib::FileEntries::FileNode* bFileEntry = (*m_package)[packageName][LotusLib::PackageTrioType::B]->getFileEntry(internalPath);
	std::unique_ptr<char[]> bDataRaw = (*m_package)[packageName][LotusLib::PackageTrioType::B]->getDataAndDecompress(bFileEntry);
	if (bDataRaw)
	{
		debugPath.replace_filename(internalPath.stem().string() + "_B");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write(bDataRaw.get(), bFileEntry->getLen());
		out.close();
	}

	const LotusLib::FileEntries::FileNode* fFileEntry = (*m_package)[packageName][LotusLib::PackageTrioType::F]->getFileEntry(internalPath);
	std::unique_ptr<char[]> fDataRaw = (*m_package)[packageName][LotusLib::PackageTrioType::F]->getDataAndDecompress(fFileEntry);
	if (fDataRaw)
	{
		debugPath.replace_filename(internalPath.stem().string() + "_F");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write(fDataRaw.get(), fFileEntry->getLen());
		out.close();
	}
}
