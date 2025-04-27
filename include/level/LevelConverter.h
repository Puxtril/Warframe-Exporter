#pragma once

#include <string_view>
#include <vector>
#include "LotusNotationParser.h"

#include "LotusPath.h"
#include "level/LevelStructs.h"
#include "model/ModelStructs.hpp"
#include "ExporterLogger.h"

namespace WarframeExporter::Level
{
	class LevelConverter
	{
	public:
		static void convertToInternal(LevelHeaderExternal& extHeader, LevelBodyExternal& extBody, const LotusLib::LotusPath& internalLevelPath, LevelInternal& intBody);
		static void replaceOverrideMaterials(const std::vector<std::string>& materialNames, Model::ModelHeaderInternal& modelHeader);
		static void convertLandscapeToInternal(const LevelExternal& levelExternal, LevelInternal& levelInternal);

	private:
		static void splitAttributes(nlohmann::json& json, LevelObjectInternal& intObj);

		// Sometimes the internal path to files are relative
		// Each level file in AnimRetarget has a corresponding folder in Misc
		// This folder has the name of the level file
		// 
		// Ex
		// AnimRetarget: /Lotus/Levels/Railjack/GrineerHangerShip/HangarPrisionInterior.level
		// Misc: /Lotus/Levels/Railjack/GrineerHangerShip/HangarPrisionInterior/
		static void fixInternalPath(const LotusLib::LotusPath& internalLevelPath, std::string& outPath);
	};
};
