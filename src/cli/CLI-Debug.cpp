#include "cli/CLI-Debug.h"

CLIDebug::CLIDebug()
{
	m_printEnums = std::make_shared<TCLAP::SwitchArg>("", "print-enums", "Print file enums", false);
	m_writeRaw = std::make_shared<TCLAP::SwitchArg>("", "write-raw", "Write unprocessed decompressed file(s)", false);
	m_debugTextCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-textures", "Attempts to read textures", false);
	m_debugModelCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-models", "Attempts to read models", false);
	m_debugMatCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-materials", "Attempts to read materials", false);
	m_debugLevelCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-levels", "Attempts to read levels", false);
	m_debugAudioCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-audio", "Attempts to read audios", false);
	m_debugShaderCmd = std::make_shared<TCLAP::SwitchArg>("", "debug-shaders", "Attempts to read shaders", false);
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
		.add(m_debugAudioCmd.get())
		.add(m_debugShaderCmd.get());
}

void
CLIDebug::addMiscCmds(TCLAP::CmdLine& cmdLine)
{
}

void
CLIDebug::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, const WarframeExporter::Ensmallening& ensmallening)
{
	if (m_debugShaderCmd->getValue() || m_debugAudioCmd->getValue() || m_debugMatCmd->getValue() || m_debugModelCmd->getValue() || m_debugTextCmd->getValue() || m_debugLevelCmd->getValue())
	{
		debug(cacheDirPath, pkgName, internalPath, outPath, ensmallening);
	}
	else if (m_printEnums->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --print-enums");
			return;
		}
		printEnums(cacheDirPath, pkgName, internalPath);
	}
	else if (m_writeRaw->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --write-raw");
			return;
		}
		if (internalPath == LotusLib::LotusPath("/"))
		{
			WarframeExporter::Logger::getInstance().error("Must use --internal-path with --write-raw");
			return;
		}
		writeRaw(outPath, internalPath, pkgName, cacheDirPath);
	}
}

void
CLIDebug::printEnums(const std::filesystem::path& cacheDirPath, const std::string& pkgName, const LotusLib::LotusPath& internalPath)
{
	static WarframeExporter::Ensmallening ensmall(true, true, true);

	if (m_printEnums->getValue())
	{
		if (pkgName.empty())
		{
			WarframeExporter::Logger::getInstance().error("Must use --package with --print-enums");
			return;
		}
		WarframeExporter::BatchIteratorDebug debugger;
		LotusLib::PackagesReader pkgs(cacheDirPath);
		LotusLib::PackageReader pkg = pkgs.getPackage(pkgName);
		debugger.printEnumCounts(pkg, internalPath);
	}
}

void
CLIDebug::writeRaw(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath)
{
	WarframeExporter::Ensmallening ensmall(true, true, true);
	WarframeExporter::BatchIteratorDebug debugger;

	LotusLib::PackagesReader pkgs(cacheDirPath);
	LotusLib::PackageReader pkg = pkgs.getPackage(pkgName);

	try
	{
		// Test if `internalPath` is a directory
		pkg.getDirMeta(internalPath);

		for (auto iter = pkg.getIter(internalPath); iter != pkg.getIter(); iter++)
		{
			LotusLib::FileEntry fileEntry = pkg.getFile(*iter);
			debugger.writeAllDebugs(pkg, fileEntry, outPath);
		}
	}
	catch (std::exception&)
	{
		LotusLib::FileEntry fileEntry = pkg.getFile(internalPath);
		debugger.writeAllDebugs(pkg, fileEntry, outPath);
	}
}

void
CLIDebug::debug(const std::filesystem::path& cacheDirPath, const std::string& pkgName, const LotusLib::LotusPath& intPath, const std::filesystem::path outPath, const WarframeExporter::Ensmallening& ensmallening)
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
	if (m_debugShaderCmd->getValue())
		types |= (int)WarframeExporter::ExtractorType::Shader;

	std::vector<std::string> pkgNames;
	if (pkgName.empty())
		pkgNames = getPkgsNames((WarframeExporter::ExtractorType)types, cacheDirPath);
	else
		pkgNames = { pkgName };

	WarframeExporter::BatchIteratorDebug debugger;
	LotusLib::PackagesReader pkgs(cacheDirPath);

	debugger.batchIterate(pkgs, ensmallening, outPath / "Debug", intPath, {pkgName}, (WarframeExporter::ExtractorType)types);
}

std::vector<std::string>
CLIDebug::getPkgsNames(WarframeExporter::ExtractorType types, const std::filesystem::path& cacheDirPath)
{
	LotusLib::PackagesReader pkgs(cacheDirPath);

	std::vector<std::string> pkgNames;
	if ((int)types & (int)WarframeExporter::ExtractorType::Texture)
	{
		try
		{
			pkgs.getPackage("Texture");
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
	
	if ((int)types & (int)WarframeExporter::ExtractorType::Shader)
	{
		pkgNames.push_back("ShaderDx11");
		pkgNames.push_back("ShaderPermutationDx11");
	}

	return pkgNames;
}
