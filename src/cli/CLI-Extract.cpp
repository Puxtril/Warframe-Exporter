#include "cli/CLI-Extract.h"

CLIExtract::CLIExtract()
	: m_dryRun(false)
{
	m_extTextCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-textures", "Extract 2D textures", false);
	m_extModelCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-models", "Extract 3D models", false);
	m_extMatCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-materials", "Extract material metadata", false);
	m_extAudioCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-audio", "Extract audio clips", false);
	m_extAllCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-all", "Extract all resource types", false);
	m_extLevelCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-levels", "Extract levels", false);
	m_extShaderCmd = std::make_shared<TCLAP::SwitchArg>("", "extract-shaders", "Extract shaders", false);
	m_extLandscape = std::make_shared<TCLAP::SwitchArg>("", "extract-landscapes", "Extract landscapes", false);
	m_extLevelStatic = std::make_shared<TCLAP::SwitchArg>("", "extract-levelstatic", "Extract static levels", false);

	m_includeVertexColors = std::make_shared<TCLAP::SwitchArg>("", "vertex-colors", "Include Vertex Colors on 3D models", false);
	m_shaderExportType = std::make_shared<TCLAP::ValueArg<std::string>>("", "shader-format", "Shader export format", false, "Binary", "Binary | Decompiled");
	m_textureFormat = std::make_shared<TCLAP::ValueArg<std::string>>("", "texture-format", "Texture output format", false, "DDS", "DDS | PNG | TGA");
	m_levelHlodExport = std::make_shared<TCLAP::ValueArg<std::string>>("", "level-hlod", "Extract HLOD files in levels", false, "Ignore", "Ignore | Include | Only");
}

CLIExtract*
CLIExtract::getInstance()
{
	static CLIExtract* instance = new CLIExtract();
	return instance;
}

void
CLIExtract::setDryRun(bool dryRun)
{
	m_dryRun = dryRun;
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
		.add(m_extShaderCmd.get())
		.add(m_extLandscape.get())
		.add(m_extLevelStatic.get());
}

void
CLIExtract::addMiscCmds(TCLAP::CmdLine& cmdLine)
{
	cmdLine
	.add(m_includeVertexColors.get())
	.add(m_shaderExportType.get())
	.add(m_textureFormat.get())
	.add(m_levelHlodExport.get());
}
 
void
CLIExtract::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, const std::filesystem::path& cacheDirPath, LotusLib::Game game)
{
	if (!m_extLevelStatic->getValue() && !m_extLandscape->getValue() && !m_extShaderCmd->getValue() && !m_extTextCmd->getValue() && !m_extModelCmd->getValue() && !m_extMatCmd->getValue() && !m_extAudioCmd->getValue() && !m_extLevelCmd->getValue() && !m_extAllCmd->getValue())
		return;

	WarframeExporter::ExtractOptions options;

	options.extractVertexColors = m_includeVertexColors->getValue();
	options.dryRun = m_dryRun;
	options.shaderExportType = getShaderFormat(m_shaderExportType->getValue());
	options.textureExportType = getTextureFormat(m_textureFormat->getValue());
	options.levelHlodExtractMode = getLevelHlodMode(m_levelHlodExport->getValue());

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
	if (m_extLandscape->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Landscape;
	if (m_extLevelStatic->getValue() || m_extAllCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::LevelStatic;

	// Debug information
	WarframeExporter::Logger::getInstance().debug("Type Flags: " + std::to_string(types));
	std::stringstream pkgs;

	extract(cacheDirPath, internalPath, outPath, (WarframeExporter::ExtractorType)types, game, options);
	WarframeExporter::Logger::getInstance().info("Extraction completed successfully");
}

WarframeExporter::Shader::ShaderExportType
CLIExtract::getShaderFormat(const std::string& cmdValue)
{
	WarframeExporter::Shader::ShaderExportType requestedType = WarframeExporter::Shader::SHADER_EXPORT_BINARY;
	if (cmdValue == "Decompiled" || cmdValue == "decompiled" || cmdValue == "decompile" || cmdValue == "Decompile")
		requestedType = WarframeExporter::Shader::SHADER_EXPORT_D3DDECOMPILE;

#ifdef Win32
	return requestedType;
#else
	if (requestedType == WarframeExporter::Shader::SHADER_EXPORT_D3DDECOMPILE)
	{
		WarframeExporter::Logger::getInstance().error("Cannot decompile shader on non-Windows OS");
		exit(1);
	}
	return WarframeExporter::Shader::SHADER_EXPORT_BINARY;
#endif
}

WarframeExporter::Texture::TextureExportType
CLIExtract::getTextureFormat(const std::string& commandValue)
{
	if (commandValue == "DDS" || commandValue == "dds")
		return WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_DDS;
	else if (commandValue == "PNG" || commandValue == "png")
		return WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_PNG;
	else if (commandValue == "TGA" || commandValue == "tga")
		return WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_TGA;
	
	WarframeExporter::Logger::getInstance().error("Texture format is not valid");
	exit(1);
}

WarframeExporter::Level::LevelHlodExtractMode
CLIExtract::getLevelHlodMode(const std::string commandValue)
{
	if (commandValue == "Ignore" || commandValue == "ignore" || commandValue == "IGNORE")
		return WarframeExporter::Level::LevelHlodExtractMode::IGNORE_HLOD;
	if (commandValue == "Include" || commandValue == "include" || commandValue == "INCLUDE")
		return WarframeExporter::Level::LevelHlodExtractMode::INCLUDE_HLOD;
	if (commandValue == "Only" || commandValue == "only" || commandValue == "ONLY")
		return WarframeExporter::Level::LevelHlodExtractMode::ONLY_HLOD;

	WarframeExporter::Logger::getInstance().error("Level HLOD export format isn't valid");
	exit(1);
}

void
CLIExtract::checkOutputDir(const std::string& outPath)
{
	if (std::filesystem::exists(outPath) && !std::filesystem::is_directory(outPath))
	{
		WarframeExporter::Logger::getInstance().error("Output directory is not valid");
		exit(1);
	}
}

void
CLIExtract::extract(const std::filesystem::path& cacheDirPath, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types, LotusLib::Game game, WarframeExporter::ExtractOptions options)
{
	WarframeExporter::BatchIteratorExport extractor;
	LotusLib::PackagesReader pkgs(cacheDirPath, game);

	if (tryExtractFile(pkgs, intPath, outPath, types, game, options))
		return;
	extractor.batchIterate(pkgs, outPath, intPath, types, game, options);
}

bool
CLIExtract::tryExtractFile(LotusLib::PackagesReader& pkgs, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types, LotusLib::Game game, WarframeExporter::ExtractOptions options)
{
	LotusLib::PackageCategory pkgCategory = WarframeExporter::g_enumMapExtractor.getPkgCategories(game, types);
	for (std::string& curPkgName : pkgs)
	{
		std::optional<LotusLib::PackageReader> curPkg = pkgs.getPackage(curPkgName);

		if (((int)curPkg->getPkgCategory() & (int)pkgCategory) == 0)
		{
			// Package not needed by specified extractors
			continue;
		}

		try
		{
			const LotusLib::FileEntries::FileNode* fileNode = (*curPkg).getFileNode(intPath);
			WarframeExporter::extractFile(pkgs, curPkgName, fileNode, outPath, game, options);
			return true;
		}
		catch (std::exception&)
		{
			continue;
		}
	}
	
	return false;
}