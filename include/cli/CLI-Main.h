#pragma once

#include "cli/CLI-Feature.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"
#include "ExporterLogger.h"

#include "LotusLib/PackageSplit.h"
#include "LotusLib/PackageCollection.h"

#include <filesystem>
#include <memory>

class CLIMain : public CLIFeature
{	
	CLIMain();

public:
	CLIMain(const CLIMain& other) = delete;
	CLIMain& operator=(const CLIMain& other) = delete;
	static CLIMain* getInstance();

	const std::string& getFeatureName();
	void addMainCmds(TCLAP::OneOf& oneOfCmd);
	void addMiscCmds(TCLAP::CmdLine& cmdLine);
	void processCmd(const std::filesystem::path& outPath, const std::string& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game) override;
};