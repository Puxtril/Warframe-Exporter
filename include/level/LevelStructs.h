#pragma once

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>
#include <map>

namespace WarframeExporter::Level
{
	struct LevelObjectHeaderExternal
	{
		std::string objTypePath;
		glm::vec3 pos;
		glm::vec4 rot;
		std::string objName;
		uint32_t meshInstanceIndex;
		uint32_t unk2;
	};

	struct LevelHeaderExternal
	{
		std::vector<std::string> paths;
		std::vector<std::string> actionPaths;
		std::vector<std::string> assetPaths;
		std::vector<LevelObjectHeaderExternal> levelObjs;
	};

	struct LevelBodyExternal
	{
		// I have no idea what these are for
		// You would think there are used to index LevelObjs
		//   since this array size is always equal to the LevelObj count
		// But nooooooooooooo
		std::vector<uint32_t> unkIndices;
		std::string attributes;
	};
	
	struct LevelObjectInternal
	{
		std::string objTypePath;
		std::string objName;
		glm::vec3 pos;
		glm::vec4 rot;
		float scale;
		std::string_view meshPath;
		std::vector<std::string_view> materials;
		std::map<std::string_view, std::string_view> attributes;
	};

	// For optimization purposes, keep the entire attributes string as an std::string
	// For each level object, use std::string_view to substring this
	struct LevelInternal
	{
		std::vector<LevelObjectInternal> objs;
		std::string _rawAttributeString;
	};
}