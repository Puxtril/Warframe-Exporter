#include "CLI-Extract.h"


CLIExtract::CLIExtract()
{
	m_extTextCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-textures", "Extract all textures", false);
	m_extModelCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-models", "Extract all 3D models", false);
	m_extMatCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-materials", "Extract material metadata", false);
	m_extAllCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-all", "Extract all resources", false);
	m_extLevelCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-levels", "Extract all levels", false);
}

CLIExtract*
CLIExtract::getInstance()
{
	static CLIExtract* instance = new CLIExtract();
	return instance;
}

const std::string&
CLIExtract::getFeatureName()
{
	static const std::string name = "Extract";
	return name;
}

void
CLIExtract::addMainCmds(TCLAP::OneOf& oneOfCmd)
{
	oneOfCmd
		.add(m_extTextCmd.get())
		.add(m_extModelCmd.get())
		.add(m_extMatCmd.get())
		.add(m_extAllCmd.get())
		.add(m_extLevelCmd.get());
}

void
CLIExtract::addMiscCmds(TCLAP::CmdLine& cmdLine)
{	
}
 
void
CLIExtract::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
{
	if (!m_extTextCmd->getValue() && !m_extModelCmd->getValue() && !m_extMatCmd->getValue() && !m_extAllCmd->getValue() && !m_extLevelCmd->getValue())
		return;

	int types = 0;
	if (m_extTextCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Texture;
	if (m_extModelCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Model;
	if (m_extMatCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Material;
	if (m_extLevelCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Level;

	std::vector<std::string> pkgNames;
	if (pkg.empty())
		pkgNames = getPkgsNames((WarframeExporter::ExtractorType)types, cache);
	else
		pkgNames = { pkg };

	extract(cache, pkgNames, internalPath, outPath, (WarframeExporter::ExtractorType)types);
}

void
CLIExtract::checkOutputDir(const std::string& outPath)
{
	if (std::filesystem::exists(outPath) && !std::filesystem::is_directory(outPath))
	{
		spdlog::get("Warframe-Exporter")->error("Output directory is not valid");
		exit(1);
	}
}

void
CLIExtract::extract(LotusLib::PackageCollection<LotusLib::CachePairReader>* cache, std::vector<std::string> pkgs, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types)
{
	WarframeExporter::Ensmallening ensmall(true, true, true);
	WarframeExporter::BatchIteratorExport extractor(cache, ensmall, outPath);
	extractor.batchIterate(intPath, pkgs, types);
}

std::vector<std::string>
CLIExtract::getPkgsNames(WarframeExporter::ExtractorType types, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
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

	if ((int)types & (int)WarframeExporter::ExtractorType::Level)
	{
		pkgNames.push_back("AnimRetarget");
	}

	if ((int)types & (int)WarframeExporter::ExtractorType::Model || (int)types & (int)WarframeExporter::ExtractorType::Material)
	{
		pkgNames.push_back("Misc");
	}

	return pkgNames;
}