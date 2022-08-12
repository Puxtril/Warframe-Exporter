#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <cstdarg>

namespace WarframeExporter
{
	class Logger
	{
		std::ofstream m_logger;

	public:
		Logger(const std::string& outputPath, bool overwrite);

		void log(std::string logEntry);
		void log(std::string logentry, int debugCount, const std::string* debugs);
	};
}