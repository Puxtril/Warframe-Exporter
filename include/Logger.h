#pragma once

#include <filesystem>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace WarframeExporter
{
	//! \brief Wrapper for spdlog
	//!
	//! Must call WarframeExporter::Logger::setLogProperties to log to file, otherwise 
	//!  logging will only stream to stdout
	class Logger
	{
	public:
		//! Creates a new logger
		//! Does not throw exception if called multiple times
		void setLogProperties(const std::filesystem::path& logPath, spdlog::level::level_enum logLevel);
		//! Used to pull an existing logger
		void setLogProperties(std::string loggerName);
		
		static Logger& getInstance();

		Logger(Logger&) = delete;
		void operator=(Logger&) = delete;

		void info(std::string msg);
		void warn(std::string msg);
		void error(std::string msg);
		void debug(std::string msg);
		void trace(std::string msg);

	protected:
		inline static std::shared_ptr<spdlog::logger> _log;

	private:
		Logger();
	};
}
