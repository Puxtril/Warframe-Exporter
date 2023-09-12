#pragma once

#include "AnimationStructs.hpp"
#include <glm/gtx/quaternion.hpp>

namespace WarframeExporter::Animation
{
	class AnimationConverter
	{
	public:
		static void convertAnimation(AnimationHeaderExternal& extHeader, AnimationBodyExternal& extBody, AnimationData& outData);
		static void applyInitialTransforms(std::vector<BoneTransform>& transforms, const BoneTransform& initial, int32_t frameCount);
		static void normalizeTransofmr(BoneTransform& transform);
	};
}