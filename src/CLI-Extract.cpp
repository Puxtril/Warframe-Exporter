#include "CLI-Extract.h"
#include "model/ModelExtractor.h"
#include "tclap/SwitchArg.h"


CLIExtract::CLIExtract()
{
	m_extTextCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-textures", "Extract all textures", false);
	m_extModelCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-models", "Extract all 3D models", false);
	m_extMatCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-materials", "Extract material metadata", false);
	m_extAudioCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-audio", "Extract audio clips", false);
	m_extAllCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-all", "Extract all resources", false);
	m_extLevelCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-levels", "Extract all levels", false);
	m_extShaderCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-shaders", "Extract all shaders", false);

	m_includeVertexColors = std::make_shared<TCLAP::SwitchArg>("", "vertex-colors", "Include extraction of Vertex Colors", false);
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
		.add(m_extLevelCmd.get())
		.add(m_extAudioCmd.get())
		.add(m_extAllCmd.get())
		.add(m_extShaderCmd.get());
}

void
CLIExtract::addMiscCmds(TCLAP::CmdLine& cmdLine)
{
	cmdLine.add(m_includeVertexColors.get());
}
 
void
CLIExtract::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, const std::filesystem::path& cacheDirPath, const WarframeExporter::Ensmallening& ensmallening)
{
	if (!m_extShaderCmd->getValue() && !m_extTextCmd->getValue() && !m_extModelCmd->getValue() && !m_extMatCmd->getValue() && !m_extAudioCmd->getValue() && !m_extLevelCmd->getValue() && !m_extAllCmd->getValue())
		return;

	WarframeExporter::Model::ModelExtractor::getInstance()->m_indexVertexColors = m_includeVertexColors->getValue();

	int types = 0;
	if (m_extTextCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Texture;
	if (m_extModelCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Model;
	if (m_extMatCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Material;
	if (m_extLevelCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Level;
	if (m_extAudioCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Audio;
	if (m_extShaderCmd->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Shader;

	std::vector<std::string> pkgNames;
	if (pkg.empty())
		pkgNames = getPkgsNames((WarframeExporter::ExtractorType)types, cacheDirPath);
	else
		pkgNames = { pkg };

	if (internalPath == "/")
	{
		std::cout << "WARNING" << std::endl;
		std::cout << "You didn't specify a path, so this will proceed to extract every asset you specified." << std::endl;
		std::cout << "Consider using --internal-path to extract a specific folder" << std::endl;
		std::cout << "Press ENTER to continue. CTRL+C to quit.";
		std::cin.get();
	}

	// Debug information
	WarframeExporter::Logger::getInstance().debug("Type Flags: " + std::to_string(types));
	std::stringstream pkgs;
	for (int i = 0; i < pkgNames.size(); i++)
		pkgs << (pkgNames[i]) << " ";
	WarframeExporter::Logger::getInstance().debug("Loading packages: " + pkgs.str());

	extract(cacheDirPath, pkgNames, internalPath, outPath, (WarframeExporter::ExtractorType)types, ensmallening);
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
CLIExtract::extract(const std::filesystem::path& cacheDirPath, std::vector<std::string> pkgs, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types, const WarframeExporter::Ensmallening& ensmallening)
{
	WarframeExporter::BatchIteratorExport extractor(cacheDirPath, ensmallening, outPath);
	extractor.batchIterate(intPath, pkgs, types);
}

std::vector<std::string>
CLIExtract::getPkgsNames(WarframeExporter::ExtractorType types, const std::filesystem::path& cacheDirPath)
{
	LotusLib::PackagesReader pkgs(cacheDirPath);

	std::vector<std::string> pkgNames;
	if ((int)types & (int)WarframeExporter::ExtractorType::Texture)
	{
		try
		{
			pkgs.getPackage("Texture");
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

	if ((int)types & (int)WarframeExporter::ExtractorType::Model ||
		(int)types & (int)WarframeExporter::ExtractorType::Material ||
		(int)types & (int)WarframeExporter::ExtractorType::Audio)
	{
		pkgNames.push_back("Misc");
	}
	
	if ((int)types & (int)WarframeExporter::ExtractorType::Shader)
	{
		pkgNames.push_back("ShaderDx11");
		pkgNames.push_back("ShaderPermutationDx11");
	}

	return pkgNames;
}
