#pragma once

#include "cli/CLI-Feature.h"
#include "tclap/MultiArg.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"
#include "Ensmallening.hpp"
#include "BatchIteratorExport.h"
#include "model/ModelExtractor.h"
#include "shader/ShaderExtractor.h"
#include "texture/TextureExtractor.h"

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

	std::shared_ptr<TCLAP::SwitchArg> m_includeVertexColors;
	std::shared_ptr<TCLAP::ValueArg<std::string>> m_shaderExportType;
	std::shared_ptr<TCLAP::ValueArg<std::string>> m_textureFormat;

	CLIExtract();

public:
	CLIExtract(const CLIExtract& other) = delete;
	CLIExtract& operator=(const CLIExtract& other) = delete;
	static CLIExtract* getInstance();

	const std::string& getFeatureName() override;
	void addMainCmds(TCLAP::OneOf& oneOfCmd) override;
	void addMiscCmds(TCLAP::CmdLine& cmdLine) override;
	void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, const std::filesystem::path& cacheDirPath, const WarframeExporter::Ensmallening& ensmallening) override;

private:
	void setShaderFormat(const std::string& cmdValue);
	void setTextureFormat(const std::string& commandValue);
	void checkOutputDir(const std::string& outPath);
	void extract(const std::filesystem::path& cacheDirPath, std::vector<std::string> pkgNames, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types, const WarframeExporter::Ensmallening& ensmallening);
};
