#include "BatchExtractor.h"

using namespace WarframeExporter;

BatchExtractor::BatchExtractor(PackageReader::PackageDir* package, const Ensmallening& ensmalleningData, std::string baseOutputPath)
	: m_package(package),
	m_ensmalleningData(ensmalleningData),
	m_pathManager(baseOutputPath),
	m_enumMap(m_enumMap.getInstance()),
	m_logger(spdlog::get("Warframe-Exporter"))
{
}

void
BatchExtractor::batchExtract(std::string basePath, std::vector<std::string> packages, FileTypeInternal types)
{
	this->validatePackages(packages);
	PackageDirLimited pkgParam = PackageDirLimited(m_package);

	for (const std::string& curPackageName : packages)
	{
		const PackageReader::CachePair* curPair = (*m_package)[curPackageName][PackageReader::PackageTrioType::H];
		
		//if ((int)types & (int)FileTypeInternal::Model)
		//	indexVertexColors(&(*m_package)[curPackageName]);

		for (auto start = curPair->getIteratorRecursive(basePath), end = curPair->end(); start != end; ++start)
		{
			const Entries::FileNode* curFile = *start;
			BinaryReaderBuffered rawData(curPair->getDataAndDecompress(curFile), curFile->getLen());
			
			CommonFileHeader header;
			if (!tryReadHeader(rawData, header))
			{
				m_logger->debug("Common header error: " + curFile->getFullPath());
				continue;
			}

			try
			{
				Extractor& extractor = m_enumMap.getExtractor(static_cast<FileTypeExternal>(header.getEnum()));
				if (((int)extractor.internalType() & (int)types) == 0)
					continue;

				std::string outputPath = m_pathManager.getOutputFilePath(curFile->getFullPath(), extractor.outFileExt());
				if (existingFileIdentical(curFile->getFullPath(), outputPath, curPair, curPackageName))
				{
					m_logger->info("Identical file time, skipping: " + curFile->getFullPath());
					continue;
				}

				extractOrLog(pkgParam, curPackageName, curFile->getFullPath(), &rawData, header, outputPath,extractor);
			}
			// Files that don't match specified enums
			catch (std::out_of_range&)
			{
				m_logger->debug("Skipping file type " + std::to_string(header.getEnum()) + ": " + curFile->getFullPath());
				continue;
			}
		}
	}
}

void
BatchExtractor::extractOrLog(PackageDirLimited& pkgParam, const std::string& packageName, const std::string internalPath, BinaryReaderBuffered* hReader, const CommonFileHeader& header, const std::string& outputPath, Extractor& extractor)
{
	try
	{
		m_logger->info("Extracting " + internalPath);
		extractor.extract(header, hReader, pkgParam, packageName, internalPath, m_ensmalleningData, outputPath);
		writeFileProperties(outputPath, internalPath, packageName);
	}
	catch (not_imeplemented_error& err)
	{
		m_logger->debug("Not implemented: " + std::string(err.what()) + " " + internalPath);
	}
	catch (unknown_format_error& err)
	{
		m_logger->debug("Unknown Format: " + std::string(err.what()) + " " + internalPath);
	}
	catch (std::exception& err)
	{
		m_logger->error(std::string(err.what()) + ": " + internalPath);
	}
}

bool
BatchExtractor::existingFileIdentical(const std::string& internalPath, const std::string& outputPath, const PackageReader::CachePair* curPair, const std::string& packageName)
{
	const Entries::FileNode* internalNode = curPair->getFileEntry(internalPath);
	std::filesystem::path p(outputPath);
	
	if (!std::filesystem::exists(p))
		return false;

	int64_t existingTime = FileProperties::readWinTimeMod(p);
	// TODO: Update timestamp to int64_t
	if ((int64_t)internalNode->getTimeStamp() != existingTime)
		return false;

	return true;
}

void
BatchExtractor::validatePackages(std::vector<std::string> packages)
{
	for (auto& curPkgStr : packages)
	{
		try
		{
			(*m_package)[curPkgStr][PackageReader::PackageTrioType::H];
		}
		catch (std::exception&)
		{
			m_logger->error("Package does not exist: " + curPkgStr);
			throw std::runtime_error("Package does not exist: " + curPkgStr);
		}
	}
}

bool
BatchExtractor::tryReadHeader(BinaryReaderBuffered& rawData, CommonFileHeader& outHeader)
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

void
BatchExtractor::indexVertexColors(PackageReader::Package* pkg)
{
	// Trigger loading of package early
	(*pkg)[PackageReader::PackageTrioType::B];

	m_logger->info("Indexing Vertex Colors");
	WarframeExporter::Model::VertexColorManager::getInstance().setPackage(pkg);
	int colCount = WarframeExporter::Model::VertexColorManager::getInstance().indexColors();
	m_logger->info(spdlog::fmt_lib::format("Found {} colors", colCount));
}

void
BatchExtractor::writeFileProperties(const std::string filePath, const std::string internalPath, const std::string& packageName)
{
	const PackageReader::CachePair* pkg = (*m_package)[packageName][PackageReader::PackageTrioType::H];
	if (pkg != nullptr)
	{
		const Entries::FileNode* fileNode = pkg->getFileEntry(internalPath);
		uint64_t timeRaw = fileNode->getTimeStamp();
		FileProperties::writeWinTime(std::filesystem::path(filePath), timeRaw, timeRaw, timeRaw);
	}
}
