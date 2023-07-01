#pragma once

#include <iostream>
#include <string_view>
#include <charconv>
#include <vector>
#include <map>
#include "glm/vec3.hpp"

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
			int instance;
		};

	public:
		static void convertToInternal(LevelHeaderExternal& extHeader, LevelBodyExternal& extBody, LevelInternal& intBody);

	private:
		static std::vector<SplitAttributes> splitAttributes(const std::string& attrs);

		static bool findInt(std::string_view attrs, const std::string_view& key, int& outValue);
		static bool findFloat(std::string_view attrs, const std::string_view& key, float& outValue);
		static bool findString(std::string_view attrs, const std::string_view& key, std::string_view& outValue);
		static bool findStringArray(std::string_view attrs, const std::string_view& key, std::vector<std::string_view>& outValue);
		static bool findVec3Float(std::string_view attrs, const std::string_view& key, glm::vec3& outValue);
	};
};
