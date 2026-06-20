#pragma once

#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"
#include "nlohmann/json.hpp"

#include <string>
#include <string_view>
#include <vector>
#include <cstdint>

namespace WarframeExporter::Level
{
	struct LevelObjectHeaderExternal
	{
		std::string objTypePath;
		glm::vec3 pos;
		glm::quat rot;
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
		std::vector<std::vector<char>> attributes;
	};

	struct LevelExternal
	{
		LevelHeaderExternal header;
		LevelBodyExternal body;
		int landscapeIndex;
	};
	
	struct LevelObjectInternal
	{
		std::string objTypePath;
		std::string objName;
		glm::vec3 pos;
		glm::quat rot;
		float scale;
		std::string meshPath;
		std::vector<std::string> materials;
		nlohmann::json::object_t attributes;
	};

	struct LevelInternal
	{
		std::vector<LevelObjectInternal> objs;
	};
}
