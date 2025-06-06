#pragma once

#include "cli/CLI-Feature.h"
#include "cli/CLI-Extract.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"
#include "DebugUtils.h"

#include "CachePair.h"
#include "PackageCollection.h"

#include <filesystem>
#include <memory>
#include <vector>

class CLIDebug : public CLIFeature
{
	std::shared_ptr<TCLAP::SwitchArg> m_printEnums;
	std::shared_ptr<TCLAP::SwitchArg> m_writeRaw;
	std::shared_ptr<TCLAP::SwitchArg> m_dryRun;
	
	CLIDebug();

public:
	CLIDebug(const CLIDebug& other) = delete;
	CLIDebug& operator=(const CLIDebug& other) = delete;
	static CLIDebug* getInstance();

	const std::string& getFeatureName() override;
	void addMainCmds(TCLAP::OneOf& oneOfCmd) override;
	void addMiscCmds(TCLAP::CmdLine& cmdLine) override;
	void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game) override;

private:
	void printEnums(const std::filesystem::path& cacheDirPath, const std::string& pkgName, const LotusLib::LotusPath& internalPath, LotusLib::Game game);
	void writeRaw(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game);
};
