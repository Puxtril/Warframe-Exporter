#include "cli/CLI-Extract.h"

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
	m_shaderExportType = std::make_shared<TCLAP::ValueArg<std::string>>("", "shader-format", "Shader export format", false, "Binary", "Binary | Decompiled");
	m_textureFormat = std::make_shared<TCLAP::ValueArg<std::string>>("", "texture-format", "Texture output format", false, "DDS", "DDS | PNG | TGA");
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
	cmdLine
	.add(m_includeVertexColors.get())
	.add(m_shaderExportType.get())
	.add(m_textureFormat.get());
}
 
void
CLIExtract::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, const std::filesystem::path& cacheDirPath, const WarframeExporter::Ensmallening& ensmallening)
{
	if (!m_extShaderCmd->getValue() && !m_extTextCmd->getValue() && !m_extModelCmd->getValue() && !m_extMatCmd->getValue() && !m_extAudioCmd->getValue() && !m_extLevelCmd->getValue() && !m_extAllCmd->getValue())
		return;

	WarframeExporter::Model::ModelExtractor::getInstance()->m_indexVertexColors = m_includeVertexColors->getValue();
	setShaderFormat(m_shaderExportType->getValue());
	setTextureFormat(m_textureFormat->getValue());

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

	// Debug information
	WarframeExporter::Logger::getInstance().debug("Type Flags: " + std::to_string(types));
	std::stringstream pkgs;
	for (size_t i = 0; i < pkgNames.size(); i++)
		pkgs << (pkgNames[i]) << " ";
	WarframeExporter::Logger::getInstance().debug("Loading packages: " + pkgs.str());

	extract(cacheDirPath, pkgNames, internalPath, outPath, (WarframeExporter::ExtractorType)types, ensmallening);
}

void
CLIExtract::setShaderFormat(const std::string& cmdValue)
{
	WarframeExporter::Shader::ShaderExportType requestedType = WarframeExporter::Shader::SHADER_EXPORT_BINARY;
	if (cmdValue == "Decompiled" || cmdValue == "decompiled" || cmdValue == "decompile" || cmdValue == "Decompile")
		requestedType = WarframeExporter::Shader::SHADER_EXPORT_D3DDECOMPILE;

#ifdef Win32
	WarframeExporter::Shader::ShaderExtractor::m_shaderExportType = requestedType;
#else
	if (requestedType == WarframeExporter::Shader::SHADER_EXPORT_D3DDECOMPILE)
	{
		WarframeExporter::Logger::getInstance().error("Cannot decompile shader on non-Windows OS");
		exit(1);
	}
	WarframeExporter::Shader::ShaderExtractor::m_shaderExportType = WarframeExporter::Shader::SHADER_EXPORT_BINARY;
#endif
}

void
CLIExtract::setTextureFormat(const std::string& commandValue)
{
	if (commandValue == "DDS" || commandValue == "dds")
		WarframeExporter::Texture::TextureExtractor::getInstance()->m_exportType = WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_DDS;
	else if (commandValue == "PNG" || commandValue == "png")
		WarframeExporter::Texture::TextureExtractor::getInstance()->m_exportType = WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_PNG;
	else if (commandValue == "TGA" || commandValue == "tga")
		WarframeExporter::Texture::TextureExtractor::getInstance()->m_exportType = WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_TGA;
	else
		spdlog::get("Warframe-Exporter")->error("Texture format is not valid");
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
CLIExtract::extract(const std::filesystem::path& cacheDirPath, std::vector<std::string> pkgNames, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types, const WarframeExporter::Ensmallening& ensmallening)
{
	WarframeExporter::BatchIteratorExport extractor;
	LotusLib::PackagesReader pkgs(cacheDirPath);

	extractor.batchIterate(pkgs, ensmallening, outPath, intPath, pkgNames, types);
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
			pkgNames.push_back("LightMap");
		}
		catch (LotusLib::LotusException&)
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
		pkgNames.push_back("Misc_xx");
	}
	
	if ((int)types & (int)WarframeExporter::ExtractorType::Shader)
	{
		pkgNames.push_back("ShaderDx11");
		pkgNames.push_back("ShaderPermutationDx11");
		
		try
		{
			pkgs.getPackage("ShaderDx12");
			pkgNames.push_back("ShaderDx12");
			pkgNames.push_back("ShaderPermutationDx12");
		}
		catch (LotusLib::LotusException&) { }
	}

	return pkgNames;
}
