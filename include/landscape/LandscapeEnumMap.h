#pragma once

#include "EnumMap.h"
#include "landscape/LandscapeReader.h"

#include "landscape/types/LandscapeReader4X.h"

namespace WarframeExporter::Landscape
{
	const static EnumMap<LandscapeReader> g_enumMapLandscape = EnumMap<LandscapeReader>()
		.registerClass(LandscapeReader4X::getInstance());
}