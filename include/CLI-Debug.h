#pragma once

#include "CLI-Feature.h"
#include "tclap/ValueArg.h"
#include "tclap/CmdLine.h"
#include "BatchIteratorDebug.h"
#include "Ensmallening.hpp"

#include "CachePair.h"
#include "PackageCollection.h"

#include <filesystem>
#include <memory>
#include <vector>

class CLIDebug : public CLIFeature
{
	std::shared_ptr<TCLAP::SwitchArg> m_printEnums;
	std::shared_ptr<TCLAP::SwitchArg> m_writeRaw;
	std::shared_ptr<TCLAP::SwitchArg> m_debugTextCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_debugModelCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_debugMatCmd;
	std::shared_ptr<TCLAP::SwitchArg> m_debugAudioCmd;
	
	CLIDebug();

public:
	CLIDebug(const CLIDebug& other) = delete;
	CLIDebug& operator=(const CLIDebug& other) = delete;
	static CLIDebug* getInstance();

	const std::string& getFeatureName();
	void addMainCmds(TCLAP::OneOf& oneOfCmd);
	void addMiscCmds(TCLAP::CmdLine& cmdLine);
	void processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache, const WarframeExporter::Ensmallening& ensmallening) override;

private:
	void printEnums(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache);
	void writeRaw(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache);
	void debug(LotusLib::PackageCollection<LotusLib::CachePairReader>* cache, const std::string& pkg, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, const WarframeExporter::Ensmallening& ensmallening);
	std::vector<std::string> getPkgsNames(WarframeExporter::ExtractorType types, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache);
};
