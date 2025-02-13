#pragma once

#include "cli/CLI-Feature.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"
#include "Ensmallening.hpp"
#include "ExporterLogger.h"

#include "CachePair.h"
#include "PackageCollection.h"

#include <filesystem>
#include <memory>

class CLIMain : public CLIFeature
{
	std::shared_ptr<TCLAP::SwitchArg> m_lsCmd;
	
	CLIMain();

public:
	CLIMain(const CLIMain& other) = delete;
	CLIMain& operator=(const CLIMain& other) = delete;
	static CLIMain* getInstance();

	const std::string& getFeatureName();
	void addMainCmds(TCLAP::OneOf& oneOfCmd);
	void addMiscCmds(TCLAP::CmdLine& cmdLine);
	void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, const WarframeExporter::Ensmallening& ensmallening, LotusLib::Game game) override;
};