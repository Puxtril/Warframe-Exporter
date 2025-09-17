#pragma once

#include "EnumMapGame.h"
#include "animation/AnimationReader.h"
#include "animation/types/AnimationReader132.h"

namespace WarframeExporter::Animation
{
	const static EnumMapGame<AnimationReader> g_enumMapAnimation = EnumMapGame<AnimationReader>()
		.registerClass(AnimationReader132::getInstance());
}