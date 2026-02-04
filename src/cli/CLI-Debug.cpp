#include "cli/CLI-Debug.h"

CLIDebug::CLIDebug()
{
	m_printEnums = std::make_shared<TCLAP::SwitchArg>("", "print-enums", "Print file enums", false);
	m_writeRaw = std::make_shared<TCLAP::SwitchArg>("", "write-raw", "Write unprocessed decompressed file(s)", false);
	m_dryRun = std::make_shared<TCLAP::SwitchArg>("", "dry-run", "Extract without writing files to disk", false);
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
		.add(m_writeRaw.get());
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
