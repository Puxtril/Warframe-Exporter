#include "BatchIteratorDebug.h"

using namespace WarframeExporter;

BatchIteratorDebug::BatchIteratorDebug()
	: BatchIterator()
{
}

void
BatchIteratorDebug::processKnownFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::FileEntry& fileEntry, Extractor* extractor, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	LotusLib::FileEntry fullFileEntry = pkgs.getPackage(pkgName).getFile(fileEntry.metadata);
	try
	{
		extractor->extractDebug(fullFileEntry, pkgs, ensmalleningData);
		m_logger.info("Successfully processed: " + fullFileEntry.internalPath.string());
	}
	catch (LotusLib::DecompressionException& err)
	{
		m_logger.warn("Decompression exception: " + std::string(err.what()) + " " + fullFileEntry.internalPath.string());
	}
	catch (LotusLib::LotusException& ex)
	{
		m_logger.error(ex.what());
	}
	catch (not_imeplemented_error& err)
	{
		m_logger.warn("Not implemented: " + std::string(err.what()) + " " + fullFileEntry.internalPath.string());
		LotusLib::PackageReader pkg = pkgs.getPackage(pkgName);
		writeAllDebugs(pkg, fullFileEntry, outputPath);
	}
	catch (unknown_format_error& err)
	{
		m_logger.error("Unknown Format: " + std::string(err.what()) + " " + fullFileEntry.internalPath.string());
		LotusLib::PackageReader pkg = pkgs.getPackage(pkgName);
		writeAllDebugs(pkg, fullFileEntry, outputPath);
	}
	catch (std::runtime_error& err)
	{
		m_logger.error(std::string(err.what()) + ": " + fullFileEntry.internalPath.string());
		LotusLib::PackageReader pkg = pkgs.getPackage(pkgName);
		writeAllDebugs(pkg, fullFileEntry, outputPath);
	}
}

void
BatchIteratorDebug::processUnknownFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::FileEntry& fileEntry, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	m_logger.debug("Unknown file type " + std::to_string(fileEntry.commonHeader.type) + ": " + fileEntry.internalPath.string());
}

void
BatchIteratorDebug::processSkipFile(LotusLib::PackagesReader& pkgs, const std::string& pkgName, LotusLib::FileEntry& fileEntry, const Extractor* extractor)
{
	m_logger.debug("Skipping file type " + std::to_string(fileEntry.commonHeader.type) + " (" + extractor->getFriendlyName() + "): " + fileEntry.internalPath.string());
}

void
BatchIteratorDebug::printEnumCounts(LotusLib::PackageReader& pkg, const LotusLib::LotusPath& internalPath)
{
	std::map<uint32_t, int> enumCounts;
	std::map<uint32_t, std::vector<std::string>> enumExamples;

	m_logger.info("Collecting Common Header Format stats");
	for (auto iter = pkg.getIter(internalPath); iter != pkg.getIter(); iter++)
	{
		try
		{
			LotusLib::FileEntry curFile = pkg.getFile(*iter, LotusLib::FileEntryReaderFlags::READ_COMMON_HEADER);

			enumCounts[curFile.commonHeader.type]++;
			
			// For the first 10 files, add directly into examples
			if (enumExamples[curFile.commonHeader.type].size() < 10)
				enumExamples[curFile.commonHeader.type].push_back(curFile.internalPath);
			
			// Every 10 new files, replace an existing example
			else if (enumCounts[curFile.commonHeader.type] % 10 == 0)
			{
				int newIndex = enumCounts[curFile.commonHeader.type] % 100 / 10;
				enumExamples[curFile.commonHeader.type][newIndex] = curFile.internalPath;
			}
		}
		catch (LotusLib::DecompressionException&)
		{
			m_logger.warn("Decompress error: " + internalPath.string());
			continue;
		}
		catch (LotusLib::LotusException& ex)
		{
			m_logger.error(ex.what());
			continue;
		}
		catch (LimitException& ex)
		{
			m_logger.warn(std::string(ex.what()) + internalPath.string());
			continue;
		}
		catch (std::exception& ex)
		{
			m_logger.error(std::string(ex.what()) + ": " + internalPath.string());
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
BatchIteratorDebug::writeAllDebugs(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const std::filesystem::path& baseOutputPath)
{
	std::filesystem::path debugPath = baseOutputPath / "Debug" / fileEntry.internalPath.getPreferredPath().relative_path();
	if (!std::filesystem::exists(debugPath.parent_path()))
		std::filesystem::create_directories(debugPath.parent_path());

	if (fileEntry.headerData.getLength() > 0)
	{
		debugPath.replace_filename(fileEntry.internalPath.filename().string() + "_H");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)fileEntry.headerData.getPtr().data(), fileEntry.headerData.getLength());
		out.close();
	}

	if (fileEntry.bData.getLength() > 0)
	{
		debugPath.replace_filename(fileEntry.internalPath.filename().string() + "_B");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)fileEntry.bData.getPtr().data(), fileEntry.bData.getLength());
		out.close();
	}

	if (fileEntry.fData.getLength() > 0)
	{
		debugPath.replace_filename(fileEntry.internalPath.filename().string() + "_F");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)fileEntry.fData.getPtr().data(), fileEntry.fData.getLength());
		out.close();
	}
}
