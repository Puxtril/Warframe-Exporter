#include "CLI-Main.h"

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
CLIMain::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
{
	if (m_lsCmd->getValue())
	{
		if (pkg.empty())
			throw std::runtime_error("Must specify package with --ls");
		(*cache)[pkg][LotusLib::PackageTrioType::H]->readToc();
		(*cache)[pkg][LotusLib::PackageTrioType::H]->lsDir(internalPath);
		(*cache)[pkg][LotusLib::PackageTrioType::H]->unReadToc();
	}
}