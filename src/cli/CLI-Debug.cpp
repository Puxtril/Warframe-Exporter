#include "cli/CLI-Debug.h"

CLIDebug::CLIDebug()
{
	m_printEnums = std::make_shared<TCLAP::SwitchArg>("", "print-enums", "Print file enums", false);
	m_writeRaw = std::make_shared<TCLAP::SwitchArg>("", "write-raw", "Write unprocessed decompressed file(s)", false);
	m_dryRun = std::make_shared<TCLAP::SwitchArg>("", "dry-run", "Extract without writing files to disk", false);
	m_ls = std::make_shared<TCLAP::SwitchArg>("", "ls", "List the contents of a directory", false);
}

CLIDebug*
CLIDebug::getInstance()
{
	static CLIDebug* instance = new CLIDebug();
	return instance;
}

const std::string&
CLIDebug::getFeatureName()
{
	static const std::string name = "Debug";
	return name;
}

void
CLIDebug::addMainCmds(TCLAP::OneOf& oneOfCmd)
{
	oneOfCmd
		.add(m_printEnums.get())
		.add(m_writeRaw.get())
		.add(m_ls.get());
}

void
CLIDebug::addMiscCmds(TCLAP::CmdLine& cmdLine)
{
	cmdLine.add(m_dryRun.get());
}

void
CLIDebug::processCmd(const std::filesystem::path& outPath, const std::string& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game)
{
	if (m_printEnums->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --print-enums");
			return;
		}
		printEnums(cacheDirPath, pkgName, internalPath, game);
	}
	else if (m_writeRaw->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --write-raw");
			return;
		}
		if (internalPath == std::string("/"))
		{
			WarframeExporter::Logger::getInstance().error("Must use --internal-path with --write-raw");
			return;
		}
		if (m_dryRun->getValue())
		{
			WarframeExporter::Logger::getInstance().error("--dry-run isn't supported with --write-raw");
			return;
		}
		writeRaw(outPath, internalPath, pkgName, cacheDirPath, game);
	}
	else if (m_ls->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --ls");
			return;
		}
		ls(internalPath, pkgName, cacheDirPath, game);
	}
	else if (m_dryRun->getValue())
	{
		CLIExtract::getInstance()->setDryRun(m_dryRun->getValue());
	}
}

void
CLIDebug::printEnums(const std::filesystem::path& cacheDirPath, const std::string& pkgName, const std::string& internalPath, LotusLib::Game game)
{
	if (m_printEnums->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --print-enums");
			return;
		}
		LotusLib::PackageCollection pkgs(cacheDirPath, game);
		std::optional<LotusLib::Package> pkg = pkgs.getPackage(pkgName);
		if (!pkg)
			throw std::runtime_error("Package does not exist: " + pkgName);

		printEnumCounts(pkg.value(), internalPath);
	}
}

void
CLIDebug::writeRaw(const std::filesystem::path outPath, const std::string& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game)
{
	LotusLib::PackageCollection pkgs(cacheDirPath, game);
	
	if (!pkgs.hasPackage(pkgName))
		throw std::runtime_error("Package does not exist: " + pkgName);
	LotusLib::Package pkg = pkgs.getPackage(pkgName);

	if (pkg.dirExists(LotusLib::PkgSplitType::HEADER, internalPath))
	{
		size_t entries = 0;
		for (auto iter = pkg.getIter(internalPath); iter != pkg.getIter(); iter++)
		{
			writeAllFilesRaw(pkg, *iter, outPath);
			entries++;
		}
		WarframeExporter::Logger::getInstance().info("Wrote " + std::to_string(entries) + " raw entries");
	}
	else if (pkg.fileExists(internalPath))
	{
		writeAllFilesRaw(pkg, pkg.getFileNode(PkgSplitType::HEADER, internalPath), outPath);
		WarframeExporter::Logger::getInstance().info("Wrote raw entry to " + outPath.string());
	}
	else
	{
		WarframeExporter::Logger::getInstance().error("Internal path doesn't exist: " + internalPath);
	}
}

void
CLIDebug::ls(const std::string& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game)
{
	try
	{
		LotusLib::PackageCollection pkgs(cacheDirPath, game);
		LotusLib::Package pkg = pkgs.getPackage(pkgName);
		LotusLib::DirNode rootDir = pkg.getDirNode(LotusLib::PkgSplitType::HEADER, internalPath);

		for (const LotusLib::DirNode* curDir : rootDir.childDirs)
		{
			const std::string curDirPath = LotusLib::getFullPath(*curDir);

			char splitStr[] = {'H', '-', '-', '\0'};
			if (pkg.dirExists(PkgSplitType::BODY, curDirPath))
				splitStr[1] = 'B';
			if (pkg.dirExists(PkgSplitType::FOOTER, curDirPath))
				splitStr[2] = 'F';

			std::cout
				<< splitStr << " "
				<< std::setw(5) << std::left << "" << " "
				<< std::setw(5) << std::left << "" << " "
				<< std::setw(12) << std::left << "" << " "
				<< curDir->name
				<< std::endl;
		}
		
		char timeStrBuf[80];
		for (const LotusLib::FileNode* curFile : rootDir.childFiles)
		{
			const std::string curFilePath = LotusLib::getFullPath(*curFile);

			auto sizes = getFileSize(pkg, curFilePath);
			std::string comSize = getFileSizeStr(std::get<0>(sizes));
			std::string size = getFileSizeStr(std::get<1>(sizes));

			time_t epochTime = curFile->timeStamp / 10000000UL - 11644473600UL;
			const tm* timeInfo = gmtime(&std::max((time_t)0, epochTime));
			strftime(timeStrBuf, 80, "%h %d %G", timeInfo);

			char splitStr[] = {'H', '-', '-', '\0'};
			if (pkg.fileExists(PkgSplitType::BODY, curFilePath))
				splitStr[1] = 'B';
			if (pkg.fileExists(PkgSplitType::FOOTER, curFilePath))
				splitStr[2] = 'F';

			std::cout
				<< splitStr << " "
				<< std::setw(5) << std::left << comSize << " "
				<< std::setw(5) << std::left << size << " "
				<< std::setw(12) << std::left << timeStrBuf << " "
				<< curFile->name
				<< std::endl;
		}

		for (const LotusLib::FileNode* curFile : rootDir.childFileDupes)
		{
			const std::string curFilePath = LotusLib::getFullPath(*curFile);

			auto sizes = getFileSize(pkg, curFilePath);
			std::string comSize = getFileSizeStr(std::get<0>(sizes));
			std::string size = getFileSizeStr(std::get<1>(sizes));

			char splitStr[] = {'H', '-', '-', '\0'};
			if (pkg.fileExists(PkgSplitType::BODY, curFilePath))
				splitStr[1] = 'B';
			if (pkg.fileExists(PkgSplitType::FOOTER, curFilePath))
				splitStr[2] = 'F';

			std::cout
				<< splitStr << " "
				<< std::setw(5) << std::left << "????" << " "
				<< std::setw(5) << std::left << "????" << " "
				<< std::setw(12) << std::left << "DELETED" << " "
				<< curFile->name
				<< std::endl;
		}
	}
	catch (LotusLib::InternalDirectoryNotFound&)
	{
		std::cout << "Cannot find directory " + internalPath << std::endl;
		return;
	}
	catch (LotusLib::PackageNotFound&)
	{
		std::cout << "Unkown package " << pkgName << std::endl;
		return;
	}
}

void
CLIDebug::printEnumCounts(LotusLib::Package& pkg, const std::string& internalPath)
{
	std::map<uint32_t, int> enumCounts;
	std::map<uint32_t, std::vector<std::string>> enumExamples;

	WarframeExporter::Logger::getInstance().info("Collecting Common Header Format stats");
	for (auto iter = pkg.getIter(internalPath); iter != pkg.getIter(); iter++)
	{
		try
		{
			uint32_t type = pkg.readCommonHeaderFormat(*iter);
			enumCounts[type]++;
			
			// For the first 10 files, add directly into examples
			if (enumExamples[type].size() < 10)
				enumExamples[type].push_back(getFullPath(*iter));
			
			// Every 10 new files, replace an existing example
			else if (enumCounts[type] % 10 == 0)
			{
				int newIndex = enumCounts[type] % 100 / 10;
				enumExamples[type][newIndex] = getFullPath(*iter);
			}
		}
		catch (LotusLib::DecompressionException&)
		{
			WarframeExporter::Logger::getInstance().warn("Decompress error: " + internalPath);
			continue;
		}
		catch (LotusLib::CommonHeaderError&)
		{
			continue;
		}
		catch (LotusLib::LotusException& ex)
		{
			WarframeExporter::Logger::getInstance().error(ex.what());
			continue;
		}
		catch (BinaryReader::LimitException& ex)
		{
			WarframeExporter::Logger::getInstance().warn(std::string(ex.what()) + internalPath);
			continue;
		}
		catch (std::exception& ex)
		{
			WarframeExporter::Logger::getInstance().error(std::string(ex.what()) + ": " + internalPath);
			continue;
		}
	}

	for (auto& x : enumCounts)
	{
		std::cout << "Type " << pkg.getName() << "/" << x.first << ": " << x.second << " entries " << std::endl;
		for (auto& str : enumExamples[x.first])
			std::cout << "  " << str << std::endl;
	}
}

void
CLIDebug::writeAllFilesRaw(LotusLib::Package& pkg, const LotusLib::FileNode& fileNode, const std::filesystem::path& baseOutputPath)
{
	LotusLib::FileEntry entry = pkg.getFileEntry(fileNode);
	const std::string internalPath = LotusLib::getFullPath(fileNode);

	std::filesystem::path debugPath = baseOutputPath / "Debug" / std::filesystem::path(internalPath).relative_path();
	if (!std::filesystem::exists(debugPath.parent_path()))
		std::filesystem::create_directories(debugPath.parent_path());

	if (entry.header.getLength() > 0)
	{
		std::ofstream out;
		out.open(debugPath.string() + "_H", std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)entry.header.getPtr().data(), entry.header.getLength());
		out.close();
	}

	if (entry.body.getLength() > 0)
	{
		std::ofstream out;
		out.open(debugPath.string() + "_B", std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)entry.body.getPtr().data(), entry.body.getLength());
		out.close();
	}

	if (entry.footer.getLength() > 0)
	{
		std::ofstream out;
		out.open(debugPath.string() + "_F", std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)entry.footer.getPtr().data(), entry.footer.getLength());
		out.close();
	}
}

std::tuple<int, int>
CLIDebug::getFileSize(LotusLib::Package& pkg, const std::string& internalPath)
{
	int compressed = 0;
	int uncompressed = 0;

	auto headerNode = pkg.getFileNode(PkgSplitType::HEADER, internalPath);
	compressed += headerNode.compLen;
	uncompressed = headerNode.len;

	if (pkg.fileExists(PkgSplitType::BODY, internalPath))
	{
		auto bodyNode = pkg.getFileNode(PkgSplitType::BODY, internalPath);
		compressed += bodyNode.compLen;
		uncompressed = bodyNode.len;
	}
	if (pkg.fileExists(PkgSplitType::FOOTER, internalPath))
	{
		auto footerNode = pkg.getFileNode(PkgSplitType::FOOTER, internalPath);
		compressed += footerNode.compLen;
		uncompressed = footerNode.len;
	}
	
	return {compressed, uncompressed};
}

std::string
CLIDebug::getFileSizeStr(int size)
{
	const char* prefixes[] = { "B", "K", "M", "G" };
	int exponent = 1;

	double sizeF = size;
	while (sizeF > 1024.0)
	{
		sizeF /= 1000.0;
		exponent++;
	}

	return std::to_string((int)sizeF) + prefixes[exponent-1]; 
}