#pragma once

#if defined WIN32 || defined MINGW
	// I hate Windows
	#define WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#include <windows.h>
#endif

#include "tclap/CmdLine.h"
#include "LotusLib.h"
#include "LotusPath.h"

#include <string>
#include <filesystem>

class CLIFeature
{
protected:
	CLIFeature() = default;
	CLIFeature(const CLIFeature& other) = delete;
	CLIFeature& operator=(const CLIFeature& other) = delete;

	public:
		virtual const std::string& getFeatureName() = 0;
		virtual void addMainCmds(TCLAP::OneOf& oneOfCmd) = 0;
		virtual void addMiscCmds(TCLAP::CmdLine& cmdLine) = 0;
		virtual void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, const std::filesystem::path& cacheDirPath, LotusLib::Game game) = 0;
};
