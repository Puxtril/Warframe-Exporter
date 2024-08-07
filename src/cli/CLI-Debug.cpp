#include "cli/CLI-Debug.h"

CLIDebug::CLIDebug()
{
	m_printEnums = std::make_shared<TCLAP::SwitchArg>("", "print-enums", "Print file enums", false);
	m_writeRaw = std::make_shared<TCLAP::SwitchArg>("", "write-raw", "Write unprocessed decompressed file(s)", false);
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
		.add(m_writeRaw.get());
}

void
CLIDebug::addMiscCmds(TCLAP::CmdLine& cmdLine)
{
}

void
CLIDebug::processCmd(const std::filesystem::path& outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath, const WarframeExporter::Ensmallening& ensmallening)
{
	if (m_printEnums->getValue())
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
		LotusLib::PackagesReader pkgs(cacheDirPath);
		LotusLib::PackageReader pkg = pkgs.getPackage(pkgName);
		WarframeExporter::DebugUtils::printEnumCounts(pkg, internalPath);
	}
}

void
CLIDebug::writeRaw(const std::filesystem::path outPath, const LotusLib::LotusPath& internalPath, const std::string& pkgName, const std::filesystem::path& cacheDirPath)
{
	LotusLib::PackagesReader pkgs(cacheDirPath);
	LotusLib::PackageReader pkg = pkgs.getPackage(pkgName);

	try
	{
		// Test if `internalPath` is a directory
		pkg.getDirNode(internalPath);

		for (auto iter = pkg.getIter(internalPath); iter != pkg.getIter(); iter++)
		{
			LotusLib::FileEntry fileEntry = pkg.getFile(*iter, LotusLib::READ_H_CACHE | LotusLib::READ_B_CACHE | LotusLib::READ_F_CACHE);
			WarframeExporter::DebugUtils::writeAllDebugs(pkg, fileEntry, outPath);
		}
	}
	catch (std::exception&)
	{
		LotusLib::FileEntry fileEntry = pkg.getFile(internalPath, LotusLib::READ_H_CACHE | LotusLib::READ_B_CACHE | LotusLib::READ_F_CACHE);
		WarframeExporter::DebugUtils::writeAllDebugs(pkg, fileEntry, outPath);
	}
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
			pkgNames.push_back("LightMap");
		}
		catch (LotusLib::LotusException&)
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
		pkgNames.push_back("Misc_xx");
	}
	
	if ((int)types & (int)WarframeExporter::ExtractorType::Shader)
	{
		pkgNames.push_back("ShaderDx11");
		pkgNames.push_back("ShaderPermutationDx11");

		try
		{
			pkgs.getPackage("ShaderDx12");
			pkgNames.push_back("ShaderDx12");
			pkgNames.push_back("ShaderPermutationDx12");
		}
		catch (LotusLib::LotusException&) { }
		
	}

	return pkgNames;
}
