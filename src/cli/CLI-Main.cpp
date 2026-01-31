#include "cli/CLI-Main.h"

CLIMain::CLIMain()
{
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
}

void
CLIMain::addMiscCmds(TCLAP::CmdLine& cmdLine)
{	
}

void
CLIMain::processCmd(const std::filesystem::path& outPath, const std::string& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game)
{
}
