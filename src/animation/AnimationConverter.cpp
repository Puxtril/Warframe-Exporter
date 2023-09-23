#include "animation/AnimationConverter.h"

using namespace WarframeExporter::Animation;

void
AnimationConverter::convertAnimation(AnimationHeaderExternal& extHeader, AnimationBodyExternal& extBody, AnimationData& outData)
{
	std::vector<AnimationSkeletonInternal> newSkels(extHeader.skeletons.size());
	for (size_t skelIndex = 0; skelIndex < extHeader.skeletons.size(); skelIndex++)
	{
		AnimationSkeletonExternal& curSkel = extHeader.skeletons[skelIndex];

		AnimationSkeletonInternal newSkel;
		newSkel.name = curSkel.name;
		newSkel.bones = curSkel.bones;

		std::vector<AnimationActionInternal> newActions(curSkel.actions.size());
		for (size_t actionIndex = 0; actionIndex < curSkel.actions.size(); actionIndex++)
		{
			ActionBodyExternal& curBodyAction = extBody.actions[skelIndex][actionIndex];  
			ActionHeaderExternal& curHeadAction = curSkel.actions[actionIndex];

			AnimationActionInternal newAction;
			newAction.name = curHeadAction.name;
			newAction.frameCount = curHeadAction.frameCount;
			newAction.timeScale = curBodyAction.timeScale;

			std::vector<BoneTransform>& transforms = curBodyAction.transforms;
			BoneTransform& initialTransform = curBodyAction.initialTransform;

			applyInitialTransforms(transforms, initialTransform, curHeadAction.frameCount);
			newAction.transforms = transforms;
			newAction.initialTransform = initialTransform;

			newActions[actionIndex] = newAction;
		}

		newSkel.actions = newActions;
		newSkels[skelIndex] = newSkel;
	}
	outData.skeletons = newSkels;
}

void
AnimationConverter::applyInitialTransforms(std::vector<BoneTransform>& transforms, BoneTransform& initial, int32_t frameCount)
{
	for (size_t x = 0; x < initial.pos.size(); x++)
	{
		initial.pos[x].x *= -1;

		//initial.rot[x].x *= -1;
		//initial.rot[x].y *= -1;

		//initial.pos[x] = glm::rotate(initial.rot[x], initial.pos[x]);
	}
	
	for (size_t x = 0; x < transforms.size(); x++)
	{
		BoneTransform& curTrans = transforms[x];
		
		glm::vec3 curInitPos = initial.pos[x];
		const glm::quat& curInitRot = initial.rot[x];
		//const glm::vec3& curInitScale = initial.scale[x];
		const glm::vec3& curInitScale = glm::vec3(1.0, 1.0, 1.0);
		
		if (curTrans.pos.size() == 0)
		{
			//auto newPos = std::vector<glm::vec3>(frameCount, curInitPos);
			//curTrans.pos = newPos;
			//for (size_t y = 0; y < curTrans.pos.size(); y++)
				//curTrans.pos[y].x *= -1;
		}
		else
		{
			for (size_t y = 0; y < curTrans.pos.size(); y++)
				curTrans.pos[y] = curTrans.pos[y] + curInitPos;
		}
		if (curTrans.rot.size() == 0)
		{
			//auto newRot = std::vector<glm::quat>(frameCount, curInitRot);
			//curTrans.rot = newRot;
		}
		else
		{
			for (size_t y = 0; y < curTrans.rot.size(); y++)
			{
				//curTrans.rot[y] *= curInitRot;
				//curTrans.rot[y].x *= -1;
				//curTrans.rot[y].y *= -1;
			}
		}

		if (curTrans.scale.size() == 0)
		{
			auto newScale = std::vector<glm::vec3>(frameCount, curInitScale);
			curTrans.scale = newScale;
		}
		else
		{
			for (size_t y = 0; y < curTrans.scale.size(); y++)
				curTrans.scale[y] = curTrans.scale[y] * curInitScale;
		}
	}
	
}