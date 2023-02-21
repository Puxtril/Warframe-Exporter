#include "CLI.h"

int
main(int argc, char** argv)
{
	// Version string + features
	std::string displayVersion = g_version.data();
	for (CLIFeature* feat : g_features)
		displayVersion += (" <" + feat->getFeatureName() + ">");

	// Main cmds
	TCLAP::CmdLine cmd(g_description.data(), ' ', displayVersion);
	TCLAP::OneOf cmds;

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

	// Basic checks
	checkDirs(cacheDirCmd.getValue(), outPathCmd.getValue());
	createLoggers(g_logLevel, outPathCmd.getValue());

	LotusLib::PackageCollection<LotusLib::CachePairReader> cacheDir(cacheDirCmd.getValue(), true);

	// Parse command
	for (CLIFeature* feat : g_features)
		feat->processCmd(outPathCmd.getValue(), intPathCmd.getValue(), pkgCmd.getValue(), &cacheDir);
}

void
addMainCommands(TCLAP::OneOf& oneOfCmd, TCLAP::CmdLine& cmdLine)
{
	TCLAP::SwitchArg lsCmd("", "ls", "List contents of directory", false);
	oneOfCmd.add(lsCmd);
}

void
checkDirs(const std::filesystem::path& cacheDir, const std::filesystem::path& outPath)
{
	if (std::filesystem::exists(outPath) && !std::filesystem::is_directory(outPath))
	{
		std::cout << "Output directory is not valid" << std::endl;
		exit(1);
	}
	if (std::filesystem::is_regular_file(cacheDir))
	{
		std::cout << "Do not select a file, please select the Cache.Windows directory" << std::endl;
		exit(1);
	}
	if (!std::filesystem::is_directory(cacheDir))
	{
		std::cout << "Not valid, please select the Cache.Windows directory" << std::endl;
		exit(1);
	}
}

void
createLoggers(spdlog::level::level_enum logLevel, const std::filesystem::path& outPath)
{
	//LotusLib::Logger::getInstance().setLogProperties("LotusLib.log", outPath, logLevel);
	WarframeExporter::Logger::getInstance().setLogProperties(outPath / "Warframe-Exporter.log", logLevel);
}