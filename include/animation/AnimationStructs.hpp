#pragma once

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

#include <string>
#include <cstdint>
#include <vector>


namespace WarframeExporter::Animation
{		
	struct BoneTreeNodeExternal
	{
		std::string name;
		int32_t parentIndex;
	};

	struct ActionHeaderExternal
	{
		std::string name;
		uint32_t frameOffset;
		uint32_t frameStride;
		uint32_t frameCount;
	};

	struct AnimationSkeletonExternal
	{
		std::string name;
		std::vector<BoneTreeNodeExternal> bones;
		uint32_t frameDataLen;
		std::vector<ActionHeaderExternal> actions;
	};

	struct AnimationHeaderExternal
	{
		std::vector<AnimationSkeletonExternal> skeletons;
	};

	struct BoneTransform
	{
		std::vector<glm::vec3> pos;
		std::vector<glm::quat> rot;
		std::vector<glm::vec3> scale;
	};

	struct ActionBodyExternal
	{
		BoneTransform initialTransform;
		std::vector<BoneTransform> transforms; // len = bone count
		std::vector<float> timeScale; // len = frame count
	};

	struct AnimationBodyExternal
	{
		std::vector<std::vector<ActionBodyExternal>> actions; // 1st len: skeleton count, 2nd len: action count
	};

	struct AnimationActionInternal
	{
		std::string name;
		uint32_t frameCount;
		BoneTransform initialTransform; // len = bone count
		std::vector<BoneTransform> transforms; // len = bone count. Each BoneTransform has len of Framecount
		std::vector<float> timeScale; // len = frame count
	};

	struct AnimationSkeletonInternal
	{
		std::string name;
		std::vector<BoneTreeNodeExternal> bones;
		std::vector<AnimationActionInternal> actions;
	};

	struct AnimationData
	{
		std::vector<AnimationSkeletonInternal> skeletons;
	};
}