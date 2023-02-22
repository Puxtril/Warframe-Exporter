#include "CLI-Debug.h"

CLIDebug::CLIDebug()
{
	m_printEnums = std::make_shared<TCLAP::SwitchArg>("", "print-enums", "Print file enums", false);
	m_debugTextCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-textures", "Attempts to read textures", false);
	m_debugModelCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-models", "Attempts to read models", false);
	m_debugMatCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-materials", "Attempts to read materials", false);
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
		.add(m_debugTextCmd.get())
		.add(m_debugModelCmd.get())
		.add(m_debugMatCmd.get());
}

void
CLIDebug::addMiscCmds(TCLAP::CmdLine& cmdLine)
{
}

void
CLIDebug::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
{
	int types = 0;
	if (m_debugTextCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Texture;
	if (m_debugModelCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Model;
	if (m_debugMatCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Material;

	std::vector<std::string> pkgNames;
	if (pkg.empty())
		pkgNames = getPkgsNames((WarframeExporter::ExtractorType)types, cache);
	else
		pkgNames = { pkg };

	debug(cache, pkgNames, internalPath, outPath, (WarframeExporter::ExtractorType)types);
}

void
CLIDebug::printEnums(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
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

void
CLIDebug::debug(LotusLib::PackageCollection<LotusLib::CachePairReader>* cache, std::vector<std::string> pkgs, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types)
{
	WarframeExporter::Ensmallening ensmall(true, true, true);
	WarframeExporter::BatchIteratorDebug debugger(cache, ensmall, outPath);
	debugger.batchIterate(intPath, pkgs, types);
}

std::vector<std::string>
CLIDebug::getPkgsNames(WarframeExporter::ExtractorType types, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
{
	std::vector<std::string> pkgNames;
	if ((int)types & (int)WarframeExporter::ExtractorType::Texture)
	{
		try
		{
			(*cache)["Texture"];
			pkgNames.push_back("Texture");
		}
		catch (std::out_of_range&)
		{
			pkgNames.push_back("TextureDx9");
		}
	}

	if ((int)types & (int)WarframeExporter::ExtractorType::Model || (int)types & (int)WarframeExporter::ExtractorType::Material)
	{
		pkgNames.push_back("Misc");
	}

	return pkgNames;
}