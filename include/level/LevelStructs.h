#pragma once

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include <string>
#include <vector>
#include <cstdint>

namespace WarframeExporter::Level
{
	struct LevelObjectHeaderExternal
	{
		std::string objPath;
		glm::vec3 pos;
		glm::vec4 rot;
		uint32_t unk1;
		uint32_t unk2;
	};

	struct LevelHeaderExternal
	{
		std::vector<std::string> paths;
		std::vector<std::string> actionPaths;
		std::vector<std::string> assetPaths;
		std::vector<LevelObjectHeaderExternal> levelObjs;
	};
}
