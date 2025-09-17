#pragma once

#include "LotusLib.h"
#include "ExporterLogger.h"
#include "EnumMapExtractor.h"

#include <vector>
#include <map>
#include <iostream>
#include <filesystem>

namespace WarframeExporter
{
	class DebugUtils
	{
	public:
		static void printEnumCounts(LotusLib::PackageReader& pkg, const LotusLib::LotusPath& internalPath);
		static void writeAllDebugs(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const std::filesystem::path& baseOutputPath);
	};
}
