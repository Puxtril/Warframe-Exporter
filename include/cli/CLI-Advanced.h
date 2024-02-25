#pragma once

#include "cli/CLI-Feature.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"
#include "BatchIteratorDebug.h"
#include "Ensmallening.hpp"

#include "CachePair.h"
#include "PackageCollection.h"

#include <filesystem>
#include <memory>
#include <vector>

class CLIAdvanced : public CLIFeature
{
	TCLAP::EitherOf m_postEnsmalleningLevel;
	std::shared_ptr<TCLAP::SwitchArg> m_isPreEnsmallening1;
	std::shared_ptr<TCLAP::SwitchArg> m_isPreEnsmallening2;
	std::shared_ptr<TCLAP::SwitchArg> m_isPreEnsmallening3;
	
	CLIAdvanced();

public:
	CLIAdvanced(const CLIAdvanced& other) = delete;
	CLIAdvanced& operator=(const CLIAdvanced& other) = delete;
	static CLIAdvanced* getInstance();

	const std::string& getFeatureName();
	void addMainCmds(TCLAP::OneOf& oneOfCmd);
	void addMiscCmds(TCLAP::CmdLine& cmdLine);
	void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, const WarframeExporter::Ensmallening& ensmallening) override;

private:

};
