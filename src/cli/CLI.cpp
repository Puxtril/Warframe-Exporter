#include "cli/CLI.h"

int
main(int argc, char** argv)
{
	if (checkIfDoubleClicked())
		exit(1);
	
	// Version string + features
	std::string displayVersion = g_version.data();
	for (CLIFeature* feat : g_features)
		displayVersion += (" <" + feat->getFeatureName() + ">");

	// Main cmds
	TCLAP::CmdLine cmd(g_description.data(), ' ', displayVersion);
	TCLAP::OneOf cmds;

	// Set custom output
	ExporterStdOutput StdOutput;
	cmd.setOutput(&StdOutput);

	for (CLIFeature* feat : g_features)
		feat->addMainCmds(cmds);

	cmd.add(cmds);

	// Add flags
	TCLAP::ValueArg<std::string> outPathCmd("", "output-path", "Destination of extracted assets. Ex: C:\\Users\\Puxtril\\Downloads\\Extracted (Default: Current directory)", false, "Extracted", "Output path");
	TCLAP::ValueArg<std::string> intPathCmd("", "internal-path", "Internal path base. Ex: /Lotus/Characters/Tenno/Excalibur", false, "/", "Internal Path");
	TCLAP::ValueArg<std::string> pkgCmd("", "package", "Warframe package. Ex: \"Misc\" or \"Texture\"", false, "", "Package name");
	TCLAP::ValueArg<std::string> cacheDirCmd("", "cache-dir", "Cache directory. Ex: C:\\Program Files\\Steam\\steamapps\\common\\Warframe\\Cache.Windows", true, "", "Cache.Windows path");
	cmd.add(intPathCmd).add(pkgCmd).add(cacheDirCmd).add(outPathCmd);

	for (CLIFeature* feat : g_features)
		feat->addMiscCmds(cmd);

	// Parse CLI parameters
	try {
		cmd.parse(argc, argv);
	}
	catch (TCLAP::ArgException& e)
	{
		std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
		exit(1);
	}
	catch (std::exception& e)
	{
		std::cerr << "error: " << e.what() << std::endl;
		exit(1);
	}

	// Basic checks
	checkDirs(cacheDirCmd.getValue());
	createLoggers(g_logLevel, outPathCmd.getValue());
	LotusLib::LotusPath fixedPath = forgiveLotusPath(intPathCmd.getValue());

	// Basic logs
	WarframeExporter::Logger::getInstance().debug(("Version: " + std::string(g_version)));

	std::stringstream args;
	for (int i = 0; i < argc; i++)
		args << (argv[i]) << " ";
	WarframeExporter::Logger::getInstance().debug(("Args: " + args.str()));

	// Create base objects
	//LotusLib::PackagesReader pkgsReader(cacheDirCmd.getValue());
	WarframeExporter::Ensmallening ensmallening(true, true, true);

	// Parse commands
	try
	{
		for (CLIFeature* feat : g_features)
			feat->processCmd(outPathCmd.getValue(), fixedPath, pkgCmd.getValue(), cacheDirCmd.getValue(), ensmallening);
	}
	catch (std::exception& e)
	{
		std::cout << std::endl;
		WarframeExporter::Logger::getInstance().error(e.what());
		exit(1);
	}
}

void
addMainCommands(TCLAP::OneOf& oneOfCmd, TCLAP::CmdLine& cmdLine)
{
	TCLAP::SwitchArg lsCmd("", "ls", "List contents of directory", false);
	oneOfCmd.add(lsCmd);
}

void
checkDirs(const std::filesystem::path& cacheDir)
{
	if (!std::filesystem::is_directory(cacheDir))
	{
		std::cout << "Not valid, please select the Cache.Windows directory" << std::endl << std::endl;

		std::cout << "It's possible the path to Cache.Windows contains a space. In which case, put quotes around the path." << std::endl << std::endl;

		std::cout << "C:\\Program Files\\Steam\\steamapps\\common\\Warframe\\Cache.Windows" << std::endl;
		std::cout << "Should be..." << std::endl;
		std::cout << "\"C:\\Program Files\\Steam\\steamapps\\common\\Warframe\\Cache.Windows\"" << std::endl;
		exit(1);
	}
}

bool
checkIfDoubleClicked()
{
	#ifdef WINDOWS

	DWORD procIDs[2];
    DWORD maxCount = 2;
    DWORD result = GetConsoleProcessList((LPDWORD)procIDs, maxCount);

	constexpr LPCWSTR msg = L"This is a console program. It needs to be executed inside a terminal (like Powershell).";
	constexpr LPCWSTR title = L"Error";

	if (result == 1)
	{
		MessageBox(NULL, msg, title, MB_ICONERROR | MB_OK);
		return true;
	}

	#endif

	return false;
}

LotusLib::LotusPath
forgiveLotusPath(LotusLib::LotusPath inPath)
{
	if (inPath.string().size() == 1)
		return inPath;

	std::basic_stringstream<std::filesystem::path::value_type> fixedPath;
	for(std::filesystem::path::string_type token : inPath)
	{
		if (token.find_first_of(':') != std::string::npos)
			continue;
		if (token.size() == 1 && token[0] == '/')
			continue;
		if (token.size() == 0)
			continue;
		fixedPath << "/" << token;
	}
	return fixedPath.str();
}

void
createLoggers(spdlog::level::level_enum logLevel, const std::filesystem::path& outPath)
{
#ifdef WF_DEBUG
	LotusLib::Logger::setLogProperties(outPath / "LotusLib.log", logLevel, logLevel);
#endif
	WarframeExporter::Logger::getInstance().setLogProperties(outPath / "Warframe-Exporter.log", logLevel);
}
