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
CLIDebug::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game)
{
	if (m_printEnums->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --print-enums");
			return;
		}
		printEnums(cacheDirPath, pkgName, internalPath);
	}
	else if (m_writeRaw->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --write-raw");
			return;
		}
		if (internalPath == LotusLib::LotusPath("/"))
		{
			WarframeExporter::Logger::getInstance().error("Must use --internal-path with --write-raw");
			return;
		}
		if (m_dryRun->getValue())
		{
			WarframeExporter::Logger::getInstance().error("--dry-run isn't supported with --write-raw");
			return;
		}
		writeRaw(outPath, internalPath, pkgName, cacheDirPath);
	}
	else if (m_dryRun->getValue())
	{
		CLIExtract::getInstance()->setDryRun(m_dryRun->getValue());
	}
}

void
CLIDebug::printEnums(const std::filesystem::path& cacheDirPath, const std::string& pkgName, const LotusLib::LotusPath& internalPath)
{
	if (m_printEnums->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --print-enums");
			return;
		}
		LotusLib::PackagesReader pkgs(cacheDirPath);
		std::optional<LotusLib::PackageReader> pkg = pkgs.getPackage(pkgName);
		if (!pkg)
			throw std::runtime_error("Package does not exist: " + pkgName);

		WarframeExporter::DebugUtils::printEnumCounts(pkg.value(), internalPath);
	}
}

void
CLIDebug::writeRaw(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath)
{
	LotusLib::PackagesReader pkgs(cacheDirPath);
	std::optional<LotusLib::PackageReader> pkg = pkgs.getPackage(pkgName);
	if (!pkg)
		throw std::runtime_error("Package does not exist: " + pkgName);

	try
	{
		// Test if `internalPath` is a directory
		pkg.value().getDirNode(internalPath);

		for (auto iter = pkg.value().getIter(internalPath); iter != pkg.value().getIter(); iter++)
		{
			LotusLib::FileEntry fileEntry = pkg.value().getFile(*iter, LotusLib::READ_H_CACHE | LotusLib::READ_B_CACHE | LotusLib::READ_F_CACHE);
			WarframeExporter::DebugUtils::writeAllDebugs(pkg.value(), fileEntry, outPath);
		}
	}
	catch (std::exception&)
	{
		LotusLib::FileEntry fileEntry = pkg.value().getFile(internalPath, LotusLib::READ_H_CACHE | LotusLib::READ_B_CACHE | LotusLib::READ_F_CACHE);
		WarframeExporter::DebugUtils::writeAllDebugs(pkg.value(), fileEntry, outPath);
	}
}
