#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "cli/CLI-Feature.h"
#include "cli/CLI-Extract.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"

#include "LotusLib/PackageSplit.h"
#include "LotusLib/PackageCollection.h"

#include <filesystem>
#include <memory>
#include <vector>
#include <time.h>

class CLIDebug : public CLIFeature
{
	std::shared_ptr<TCLAP::SwitchArg> m_printEnums;
	std::shared_ptr<TCLAP::SwitchArg> m_writeRaw;
	std::shared_ptr<TCLAP::SwitchArg> m_dryRun;
	std::shared_ptr<TCLAP::SwitchArg> m_ls;
	
	CLIDebug();

public:
	CLIDebug(const CLIDebug& other) = delete;
	CLIDebug& operator=(const CLIDebug& other) = delete;
	static CLIDebug* getInstance();

	const std::string& getFeatureName() override;
	void addMainCmds(TCLAP::OneOf& oneOfCmd) override;
	void addMiscCmds(TCLAP::CmdLine& cmdLine) override;
	void processCmd(const std::filesystem::path& outPath, const std::string& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game) override;

private:
	void printEnums(const std::filesystem::path& cacheDirPath, const std::string& pkgName, const std::string& internalPath, LotusLib::Game game);
	void writeRaw(const std::filesystem::path outPath, const std::string& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game);
	void ls(const std::string& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, LotusLib::Game game);

	void printEnumCounts(LotusLib::Package& pkg, const std::string& internalPath);
	void writeAllFilesRaw(LotusLib::Package& pkg, const LotusLib::FileNode& fileNode, const std::filesystem::path& baseOutputPath);

	static std::tuple<int, int> getFileSize(LotusLib::Package& pkg, const std::string& internalPath);
	static std::string getFileSizeStr(int size);
};
