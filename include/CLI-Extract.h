#pragma once

#include "CLI-Feature.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"
#include "Ensmallening.hpp"
#include "BatchIteratorExport.h"

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

	CLIExtract();

public:
	CLIExtract(const CLIExtract& other) = delete;
	CLIExtract& operator=(const CLIExtract& other) = delete;
	static CLIExtract* getInstance();

	const std::string& getFeatureName();
	void addMainCmds(TCLAP::OneOf& oneOfCmd) override;
	void addMiscCmds(TCLAP::CmdLine& cmdLine) override;
	void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache) override;

private:
	void checkOutputDir(const std::string& outPath);
	void extract(LotusLib::PackageCollection<LotusLib::CachePairReader>* cache, std::vector<std::string> pkgs, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, WarframeExporter::ExtractorType types);
	std::vector<std::string> getPkgsNames(WarframeExporter::ExtractorType types, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache);
};