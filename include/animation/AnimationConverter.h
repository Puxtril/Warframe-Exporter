#pragma once

#include "AnimationStructs.hpp"
#include <glm/gtx/quaternion.hpp>

namespace WarframeExporter::Animation
{
	class AnimationConverter
	{
	public:
		static void convertAnimation(AnimationHeaderExternal& extHeader, AnimationBodyExternal& extBody, AnimationData& outData);
		// Not very useful, as the initial transform doesn't usually match the model rest pose...
		static void flipInitialTransforms(BoneTransform &initial);
	};
}