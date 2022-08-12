#pragma once

#include <string>
#include <filesystem>

namespace WarframeExporter
{
	// Will create directories as needed
	// Can be thought of as a "repository manager"
	class ExportPathManager {
		std::filesystem::path m_basePath;
		std::string m_debugLogPath;
		std::string m_debugOutputPath;
		inline const static std::string m_outputDirName = "Extract";
		inline const static std::string m_debugDirName = "Debug";

	public:
		ExportPathManager(std::string basePath);

		std::string getOutputFilePath(const std::string& internalPath);
		std::string getOutputFilePath(const std::string& internalPath, const std::string& replaceExtension);
		std::string getDebugOutputFilePath(const std::string& internalPath);
		std::string getDebugOutputFilePath(const std::string& internalPath, const std::string& filePostfix);
		std::string getLogPublicFilePath();
		std::string getLogPrivateFilePath();
		std::string getLogSkipFilePath();
		std::string getLogDebugFilePath();

	private:
		std::string createOutputDebugPath();
		std::string createLogDebugFilePath();
		std::string internalToOSPath(std::string internalPath);
	};
}