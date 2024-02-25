#pragma once

#include "Meta.h"
#include "tclap/CmdLine.h"
#include "tclap/ArgException.h"
#include "tclap/UnlabeledValueArg.h"
#include "tclap/SwitchArg.h"
#include "LotusLib.h"
#include "BatchIteratorExport.h"
#include "LotusPath.h"
#include "cli/CLI-Feature.h"
#include "cli/CLI-Extract.h"
#include "cli/CLI-Debug.h"
#include "cli/CLI-Main.h"
#include "cli/CLI-Advanced.h"
#include "cli/CLI-Output.h"

#include <filesystem>
#include <string>
#include <string_view>
#include <sstream>

#if defined WIN32 || defined MINGW
	// I hate Windows
	#define WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

const static std::string_view g_description = "https://github.com/Puxtril/Warframe-Exporter";

const std::vector<CLIFeature*> g_features = {
#ifdef WF_ADVANCED
	CLIAdvanced::getInstance(),
#endif
#ifdef WF_EXTRACT
	CLIExtract::getInstance(),
#endif
#ifdef WF_DEBUG
	CLIDebug::getInstance(),
#endif
	CLIMain::getInstance()
};

int main(int argc, char** argv);

void checkDirs(const std::filesystem::path& cacheDir);
LotusLib::LotusPath forgiveLotusPath(LotusLib::LotusPath inPath);
bool checkIfDoubleClicked();
void createLoggers(spdlog::level::level_enum logLevel, const std::filesystem::path& outPath);
