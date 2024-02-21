#include "CLI-Main.h"
#include "LotusLib.h"

CLIMain::CLIMain()
{
	m_lsCmd = std::make_shared<TCLAP::SwitchArg>("", "ls", "List contents of directory", false);
}

CLIMain*
CLIMain::getInstance()
{
	static CLIMain* instance = new CLIMain();
	return instance;
}

const std::string&
CLIMain::getFeatureName()
{
	static const std::string name = "Main";
	return name;
}

void
CLIMain::addMainCmds(TCLAP::OneOf& oneOfCmd)
{
	oneOfCmd.add(m_lsCmd.get());
}

void
CLIMain::addMiscCmds(TCLAP::CmdLine& cmdLine)
{	
}

void
CLIMain::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, const WarframeExporter::Ensmallening& ensmallening)
{
	if (m_lsCmd->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must specify package with --ls");
			exit(1);
		}
		LotusLib::PackagesReader pkgs(cacheDirPath);
		LotusLib::PackageReader pkg = pkgs.getPackage(pkgName);

		pkg.lsDir(internalPath);
	}
}
