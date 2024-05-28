#pragma once

#include "EnumMap.h"
#include "landscape/LandscapeReader.h"

#include "landscape/types/LandscapeReader42.h"

namespace WarframeExporter::Landscape
{
	const static EnumMap<LandscapeReader> g_enumMapLandscape = EnumMap<LandscapeReader>()
		.registerClass(LandscapeReader42::getInstance());
}