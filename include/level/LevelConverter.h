#pragma once

#include <iostream>
#include <string_view>
#include <charconv>
#include <vector>
#include <map>
#include "glm/vec3.hpp"
#include "glm/ext/matrix_transform.hpp"

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
		static void replaceOverrideMaterials(const std::vector<std::string_view>& materialNames, Model::ModelHeaderInternal& modelHeader);
		static void applyTransformation(LevelObjectInternal& levelObj, std::vector<glm::vec3>& verts);

	private:
		static void splitAttributes(const std::string& attrs, LevelObjectInternal& intObj);

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
		static void fixInternalPath(const LotusLib::LotusPath& internalLevelPath, std::string& outPath);
	};
};
