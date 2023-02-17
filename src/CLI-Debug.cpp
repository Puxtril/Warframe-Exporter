#include "CLI-Debug.h"

CLIDebug::CLIDebug()
{
	m_printEnums = std::make_shared<TCLAP::SwitchArg>("", "print-enums", "Print file enums", false);
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
	oneOfCmd.add(m_printEnums.get());
}

void
CLIDebug::addMiscCmds(TCLAP::CmdLine& cmdLine)
{	
}

void
CLIDebug::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
{
	static WarframeExporter::Ensmallening ensmall(true, true, true);
	
	if (m_printEnums->getValue() && pkg.empty())
	{
		WarframeExporter::BatchIteratorDebug debugger(cache, ensmall, outPath);
		for (LotusLib::Package<LotusLib::CachePairReader>& x : (*cache))
		{
			std::cout << "Package: " << x.getName() << std::endl;
			debugger.printEnumCounts(x.getName(), internalPath);
			std::cout << std::endl;
		}
	}
	
	else if (m_printEnums->getValue())
	{
		WarframeExporter::BatchIteratorDebug debugger(cache, ensmall, outPath);
		debugger.printEnumCounts(pkg, internalPath);
	}
}