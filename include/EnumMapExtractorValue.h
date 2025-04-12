#pragma once

#include "Enums.h"

#include <vector>
#include <tuple>

namespace WarframeExporter
{
	enum class ExtractorType
	{
		Model = 1,
		Texture = 2,
		Material = 4,
		VERTEX_COLOR = 8,
		Level = 32,
		Audio = 64,
		Shader = 128,
		Landscape = 256,
		LevelStatic = 512,
	};

	// This is how Extractors register with specific Games and Package Categories.
	// `EnumMapExtractor` will statically check for double-registers.
    class EnumMapExtractorValue
	{
	public:
		virtual std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const = 0;
	};
};