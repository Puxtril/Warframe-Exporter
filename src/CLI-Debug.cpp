#include "CLI-Debug.h"
#include <exception>
#include <stdexcept>

CLIDebug::CLIDebug()
{
	m_printEnums = std::make_shared<TCLAP::SwitchArg>("", "print-enums", "Print file enums", false);
	m_writeRaw = std::make_shared<TCLAP::SwitchArg>("", "write-raw", "Write unprocessed decompressed file(s)", false);
	m_debugTextCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-textures", "Attempts to read textures", false);
	m_debugModelCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-models", "Attempts to read models", false);
	m_debugMatCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-materials", "Attempts to read materials", false);
	m_debugLevelCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-levels", "Attempts to read levels", false);
	m_debugAudioCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-audio", "Attempts to read audios", false);
}

CLIDebug*
CLIDebug::getInstance()
{
	static CLIDebug* instance = new CLIDebug();
	return instance;
}

const std::string&
CLIDebug::getFeatureName()
{
	static const std::string name = "Debug";
	return name;
}

void
CLIDebug::addMainCmds(TCLAP::OneOf& oneOfCmd)
{
	oneOfCmd
		.add(m_printEnums.get())
		.add(m_writeRaw.get())
		.add(m_debugTextCmd.get())
		.add(m_debugModelCmd.get())
		.add(m_debugMatCmd.get())
		.add(m_debugLevelCmd.get())
		.add(m_debugAudioCmd.get());
}

void
CLIDebug::addMiscCmds(TCLAP::CmdLine& cmdLine)
{
}

void
CLIDebug::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache, const WarframeExporter::Ensmallening& ensmallening)
{
	if (m_debugAudioCmd->getValue() || m_debugMatCmd->getValue() || m_debugModelCmd->getValue() || m_debugTextCmd->getValue() || m_debugLevelCmd->getValue())
	{
		debug(cache, pkg, internalPath, outPath, ensmallening);
	}
	else if (m_printEnums->getValue())
	{
		if (pkg.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --print-enums");
			return;
		}
		printEnums(outPath, internalPath, pkg, cache);
	}
	else if (m_writeRaw->getValue())
	{
		if (pkg.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --write-raw");
			return;
		}
		if (internalPath == LotusLib::LotusPath("/"))
		{
			WarframeExporter::Logger::getInstance().error("Must use --internal-path with --write-raw");
			return;
		}
		writeRaw(outPath, internalPath, pkg, cache);
	}
}

void
CLIDebug::printEnums(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
{
	static WarframeExporter::Ensmallening ensmall(true, true, true);

	if (m_printEnums->getValue() && pkg.empty())
	{
		WarframeExporter::BatchIteratorDebug debugger(cache, ensmall, outPath);
		for (LotusLib::Package<LotusLib::CachePairReader>& x : (*cache))
		{
			std::cout << "Package: " << x.getName() << std::endl;
			debugger.printEnumCounts(x.getName(), internalPath);
			std::cout << std::endl;
		}
	}

	else if (m_printEnums->getValue())
	{
		WarframeExporter::BatchIteratorDebug debugger(cache, ensmall, outPath);
		debugger.printEnumCounts(pkg, internalPath);
	}
}

void
CLIDebug::writeRaw(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkg, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
{
	WarframeExporter::Ensmallening ensmall(true, true, true);
	WarframeExporter::BatchIteratorDebug debugger(cache, ensmall, outPath);

	(*cache)[pkg][LotusLib::PackageTrioType::H]->readToc();
	if ((*cache)[pkg][LotusLib::PackageTrioType::B])
		(*cache)[pkg][LotusLib::PackageTrioType::B]->readToc();
	if ((*cache)[pkg][LotusLib::PackageTrioType::F])
		(*cache)[pkg][LotusLib::PackageTrioType::F]->readToc();

	try
	{
		// Test if `internalPath` is a directory
		(*cache)[pkg][LotusLib::PackageTrioType::H]->getDirEntry(internalPath);

		auto curPair = (*cache)[pkg][LotusLib::PackageTrioType::H];
		for (auto iter = curPair->getIter(internalPath); iter != curPair->getIter(); iter++)
		{
			debugger.writeAllDebugs(pkg, (*iter)->getFullPath());
		}
	}
	catch (std::exception&)
	{
		debugger.writeAllDebugs(pkg, internalPath);
	}
}

void
CLIDebug::debug(LotusLib::PackageCollection<LotusLib::CachePairReader>* cache, const std::string& pkg, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, const WarframeExporter::Ensmallening& ensmallening)
{
	int types = 0;
	if (m_debugTextCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Texture;
	if (m_debugModelCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Model;
	if (m_debugMatCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Material;
	if (m_debugLevelCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Level;
	if (m_debugAudioCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Audio;

	std::vector<std::string> pkgNames;
	if (pkg.empty())
		pkgNames = getPkgsNames((WarframeExporter::ExtractorType)types, cache);
	else
		pkgNames = { pkg };

	WarframeExporter::BatchIteratorDebug debugger(cache, ensmallening, outPath);
	debugger.batchIterate(intPath, pkgNames, (WarframeExporter::ExtractorType)types);
}

std::vector<std::string>
CLIDebug::getPkgsNames(WarframeExporter::ExtractorType types, LotusLib::PackageCollection<LotusLib::CachePairReader>* cache)
{
	std::vector<std::string> pkgNames;
	if ((int)types & (int)WarframeExporter::ExtractorType::Texture)
	{
		try
		{
			(*cache)["Texture"];
			pkgNames.push_back("Texture");
		}
		catch (std::out_of_range&)
		{
			pkgNames.push_back("TextureDx9");
		}
	}

	if ((int)types & (int)WarframeExporter::ExtractorType::Level)
	{
		pkgNames.push_back("AnimRetarget");
	}

	if ((int)types & (int)WarframeExporter::ExtractorType::Model ||
		(int)types & (int)WarframeExporter::ExtractorType::Material ||
		(int)types & (int)WarframeExporter::ExtractorType::Audio)
	{
		pkgNames.push_back("Misc");
	}

	return pkgNames;
}
