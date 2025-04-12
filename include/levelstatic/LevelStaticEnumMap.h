#pragma once

#include "EnumMap.h"
#include "levelstatic/types/LevelStaticReader28.h"

namespace WarframeExporter::LevelStatic
{
	const static EnumMap<LevelStaticReader> g_enumMapLevelStatic = EnumMap<LevelStaticReader>() 
        .registerClass(LevelStaticReader28::getInstance()
    );
}