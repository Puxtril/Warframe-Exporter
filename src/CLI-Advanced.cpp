#include "CLI-Advanced.h"

CLIAdvanced::CLIAdvanced()
{
	m_isPreEnsmallening1 = std::make_shared<TCLAP::SwitchArg>("", "pre-ensmall-1", "No ensmallening", false);
	m_isPreEnsmallening2 = std::make_shared<TCLAP::SwitchArg>("", "pre-ensmall-2", "Between part 1 and part 2", false);
	m_isPreEnsmallening3 = std::make_shared<TCLAP::SwitchArg>("", "pre-ensmall-3", "Between part 2 and part 3", false);
}

CLIAdvanced*
CLIAdvanced::getInstance()
{
	static CLIAdvanced* instance = new CLIAdvanced();
	return instance;
}

const std::string&
CLIAdvanced::getFeatureName()
{
	static const std::string name = "Advanced";
	return name;
}

void
CLIAdvanced::addMainCmds(TCLAP::OneOf& oneOfCmd)
{
}

void
CLIAdvanced::addMiscCmds(TCLAP::CmdLine& cmdLine)
{
	m_postEnsmalleningLevel.add(m_isPreEnsmallening1.get());
	m_postEnsmalleningLevel.add(m_isPreEnsmallening2.get());
	m_postEnsmalleningLevel.add(m_isPreEnsmallening3.get());

	cmdLine.add(m_postEnsmalleningLevel);
}

void
CLIAdvanced::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache, const WarframeExporter::Ensmallening& ensmallening)
{
	WarframeExporter::Ensmallening& ensmalleningMutable = const_cast<WarframeExporter::Ensmallening&>(ensmallening);

	if (m_isPreEnsmallening3->getValue())
	{
		ensmalleningMutable = WarframeExporter::Ensmallening(true, true, false);
	}
	else if (m_isPreEnsmallening2->getValue())
	{
		ensmalleningMutable = WarframeExporter::Ensmallening(true, false, false);
	}
	else if (m_isPreEnsmallening1->getValue())
	{
		// Sooo hacky
		// :(
		std::filesystem::path cacheDir = cache->getPath();
		delete cache;
		cache = new LotusLib::PackageCollection<LotusLib::CachePairReader>(cacheDir, false);

		ensmalleningMutable = WarframeExporter::Ensmallening(false, false, false);
	}
}
