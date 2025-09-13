#pragma once

#include "cli/CLI-Feature.h"
#include "EnumMapExtractor.h"
#include "ExportSingleFile.h"

#include "tclap/MultiArg.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"
#include "BatchIteratorExport.h"
#include "model/ModelExtractor.h"

#include <memory>
#include <filesystem>

class CLIExtract : public CLIFeature
{
	// operator= is deleted for these
	std::shared_ptr<TCLAP::SwitchArg> m_extAllCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_extLevelCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_extTextCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_extModelCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_extMatCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_extAudioCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_extShaderCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_extLandscape;
	std::shared_ptr<TCLAP::SwitchArg> m_extLevelStatic;

	std::shared_ptr<TCLAP::SwitchArg> m_includeVertexColors;
	std::shared_ptr<TCLAP::ValueArg<std::string>> m_shaderExportType;
	std::shared_ptr<TCLAP::ValueArg<std::string>> m_textureFormat;
	std::shared_ptr<TCLAP::ValueArg<std::string>> m_materialFormat;
	std::shared_ptr<TCLAP::ValueArg<std::string>> m_levelHlodExport;

	bool m_dryRun;

	CLIExtract();

public:
	CLIExtract(const CLIExtract& other) = delete;
	CLIExtract& operator=(const CLIExtract& other) = delete;
	static CLIExtract* getInstance();

	void setDryRun(bool dryRun);

	const std::string& getFeatureName() override;
	void addMainCmds(TCLAP::OneOf& oneOfCmd) override;
	void addMiscCmds(TCLAP::CmdLine& cmdLine) override;
	void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, const std::filesystem::path& cacheDirPath, LotusLib::Game game) override;

private:
	WarframeExporter::Shader::ShaderExportType getShaderFormat(const std::string& cmdValue);
	WarframeExporter::Texture::TextureExportType getTextureFormat(const std::string& commandValue);
	WarframeExporter::Material::MaterialExtractType getMaterialFormat(const std::string& commandValue);
	WarframeExporter::Level::LevelHlodExtractMode getLevelHlodMode(const std::string commandValue);

	void checkOutputDir(const std::string& outPath);
	void extract(const std::filesystem::path& cacheDirPath, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types, LotusLib::Game game, WarframeExporter::ExtractOptions options);
	bool tryExtractFile(LotusLib::PackagesReader& pkgs, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types, LotusLib::Game game, WarframeExporter::ExtractOptions options);
};
