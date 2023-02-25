#pragma once

#include "EnumMap.h"
#include "level/LevelReader.h"
#include "level/types/LevelReader196.h"

namespace WarframeExporter::Level
{
	const static EnumMap<LevelReader> g_enumMapLevel = EnumMap<LevelReader>()
		.registerClass(LevelReader196::getInstance());
}