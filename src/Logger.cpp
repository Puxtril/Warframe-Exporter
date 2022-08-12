#include "Logger.h"

using namespace WarframeExporter;

Logger::Logger(const std::string& outputPath, bool overwrite)
{
	if (overwrite)
		m_logger = std::ofstream(outputPath, std::ofstream::out | std::ofstream::trunc);
	else
		m_logger = std::ofstream(outputPath, std::ofstream::out | std::ofstream::app);
	if (outputPath == "")
		m_logger.setstate(std::ios_base::badbit);
}

void
Logger::log(std::string logEntry)
{
	m_logger << logEntry << std::endl;
}

void
Logger::log(std::string logentry, int debugCount, const std::string* debug)
{
	m_logger << logentry << std::endl;
	for (int x = 0; x < debugCount; x++)
		m_logger << "  " << debug[x] << std::endl;
}