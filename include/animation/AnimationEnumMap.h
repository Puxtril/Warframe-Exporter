#pragma once

#include "EnumMap.h"
#include "animation/AnimationReader.h"
#include "animation/types/AnimationReader132.h"

namespace WarframeExporter::Animation
{
	const static EnumMap<AnimationReader> g_enumMapAnimation = EnumMap<AnimationReader>()
		.registerClass(AnimationReader132::getInstance());
}