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

			flipInitialTransforms(initialTransform);

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
AnimationConverter::flipInitialTransforms(BoneTransform& initial)
{
	for (size_t x = 0; x < initial.pos.size(); x++)
	{
		initial.pos[x].x *= -1;

		initial.rot[x].x *= -1;
		initial.rot[x].y *= -1;
	}
}