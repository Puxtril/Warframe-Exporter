#include "ExportPathManager.h"

using namespace WarframeExporter;

ExportPathManager::ExportPathManager(std::string basePath)
	: m_basePath(basePath)
{
	if (!std::filesystem::is_directory(basePath) || !std::filesystem::exists(basePath))
		std::filesystem::create_directories(basePath);
	
	std::filesystem::path exportPath = std::filesystem::path(basePath) / m_outputDirName;
	if (!std::filesystem::is_directory(exportPath) || !std::filesystem::exists(exportPath))
		std::filesystem::create_directory(exportPath);

	m_debugOutputPath = createOutputDebugPath();
}

std::string
ExportPathManager::getOutputFilePath(const std::string& internalPath)
{
	std::filesystem::path newPath = std::filesystem::path(m_basePath);
	newPath /= m_outputDirName + internalToOSPath(internalPath);
	std::filesystem::create_directories(newPath.parent_path());
	return newPath.string();
}

std::string
ExportPathManager::getOutputFilePath(const std::string& internalPath, const std::string& replaceExtension)
{
	std::filesystem::path newPath = std::filesystem::path(m_basePath);
	newPath /= m_outputDirName + internalToOSPath(internalPath);
	newPath = newPath.replace_extension(replaceExtension);
	std::filesystem::create_directories(newPath.parent_path());
	return newPath.string();
}

std::string
ExportPathManager::getDebugOutputFilePath(const std::string& internalPath)
{
	std::filesystem::path newPath = m_debugOutputPath;
	newPath = newPath.concat(internalToOSPath(internalPath));
	std::filesystem::create_directories(newPath.parent_path());
	return newPath.string();
}

std::string
ExportPathManager::getDebugOutputFilePath(const std::string& internalPath, const std::string& filePostfix)
{
	std::filesystem::path newPath = internalToOSPath(internalPath);
	newPath.replace_filename(newPath.stem().string().append(filePostfix) + newPath.extension().string());
	newPath = std::filesystem::path(m_debugOutputPath).concat(newPath.string());
	std::filesystem::create_directories(newPath.parent_path());
	return newPath.string();
}

std::string
ExportPathManager::createOutputDebugPath()
{
	std::filesystem::path newPath = std::filesystem::path(m_basePath);
	int debugIndex = 1;
	while (std::filesystem::exists(newPath / ("Debug" + std::to_string(debugIndex))))
		debugIndex++;
	newPath /= ("Debug" + std::to_string(debugIndex));
	return newPath.string();
}

std::string
ExportPathManager::internalToOSPath(std::string internalPath)
{
#if defined(_WIN32) || defined(_WIN64) || defined(__CYGWIN__)
	const static char winSep = '\\';
	const static char posSep = '/';
	std::replace(internalPath.begin(), internalPath.end(), posSep, winSep);
	return internalPath;
#else
	return internalPath;
#endif
}