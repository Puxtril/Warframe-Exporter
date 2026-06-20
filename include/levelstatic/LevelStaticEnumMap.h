#pragma once

#include "EnumMap.h"
#include "levelstatic/types/LevelStaticReader28.h"
#include "levelstatic/types/LevelStaticReader29.h"
#include "levelstatic/types/LevelStaticReader34.h"

namespace WarframeExporter::LevelStatic
{
	const static EnumMap<LevelStaticReader> g_enumMapLevelStatic = EnumMap<LevelStaticReader>() 
        .registerClass(LevelStaticReader28::getInstance())
        .registerClass(LevelStaticReader29::getInstance())
        .registerClass(LevelStaticReader34::getInstance());
}