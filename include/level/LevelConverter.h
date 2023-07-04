#pragma once

#include <iostream>
#include <string_view>
#include <charconv>
#include <vector>
#include <map>
#include "glm/vec3.hpp"

#include "LotusPath.h"
#include "level/LevelStructs.h"
#include "ExporterLogger.h"

namespace WarframeExporter::Level
{
	class LevelConverter
	{
		// From a giant string of attributes...
		// Split into an array of attribute groups...
		// These are cherry-picked attributes
		struct SplitAttributes
		{
			std::string_view meshPath;
			std::vector<std::string_view> materials;
			std::map<std::string_view, std::string_view> extraAttrs;
			float scale;
			glm::vec3 posOffset;
			size_t instance;
		};

	public:
		static void convertToInternal(LevelHeaderExternal& extHeader, LevelBodyExternal& extBody, const LotusLib::LotusPath& internalLevelPath, LevelInternal& intBody);

	private:
		static std::vector<SplitAttributes> splitAttributes(const std::string& attrs);

		// This probably could be performed using Regex, but..
		//  1) std::regex is very slow
		//  2) I don't want to add a regex library for this
		//  3) There is enough extra searching/exceptions to warrant a custom implementation anyway
		//
		// For this function, simply find a matching string
		static size_t findAttribute(std::string_view attrs, const std::string_view& key);

		static bool findInt(std::string_view attrs, const std::string_view& key, size_t& outValue);
		static bool findFloat(std::string_view attrs, const std::string_view& key, float& outValue);
		static bool findString(std::string_view attrs, const std::string_view& key, std::string_view& outValue);
		static bool findStringArray(std::string_view attrs, const std::string_view& key, std::vector<std::string_view>& outValue);
		static bool findVec3Float(std::string_view attrs, const std::string_view& key, glm::vec3& outValue);


		// Sometimes the internal path to files are relative
		// Each level file in AnimRetarget has a corresponding folder in Misc
		// This folder has the name of the level file
		// 
		// Ex
		// AnimRetarget: /Lotus/Levels/Railjack/GrineerHangerShip/HangarPrisionInterior.level
		// Misc: /Lotus/Levels/Railjack/GrineerHangerShip/HangarPrisionInterior/
		static void fixInternalPath(const LotusLib::LotusPath& internalLevelPath, std::string_view readPath, std::string& outPath);
	};
};
