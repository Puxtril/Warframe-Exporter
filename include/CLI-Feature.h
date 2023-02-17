#pragma once

#include "tclap/CmdLine.h"
#include "PackageCollection.h"
#include "CachePairReader.h"
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
		virtual void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache) = 0;
};
