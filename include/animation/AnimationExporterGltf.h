#pragma once

#include "AnimationStructs.hpp"
#include "fx/gltf.h"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/quaternion.hpp"

#include <vector>
#include <cstdint>
#include <string>
#include <filesystem>

using namespace fx;

namespace WarframeExporter::Animation
{
	class gltfAnimation
	{
		gltf::Document m_document;
		inline static const std::string m_generatorName = "Puxtril's Warframe Animation Exporter";
		inline static const std::string m_copyright = "DIGITAL EXTREMES Ltd.";

	public:
		gltfAnimation();

		void addAnimationData(const AnimationData& animData);
		void save(const std::filesystem::path& outPathStr);

	private:
		void addSkeletonToScene(int32_t rootBoneIndex, int32_t boneCount);
		// Returns index of root bone in tree
		// Garuntees same order as boneTree
		int32_t createBones(const std::vector<BoneTreeNodeExternal>& boneTree, const AnimationActionInternal& action);
	
		void addActionData(const AnimationActionInternal& action, uint32_t rootNodeOffset);
		void addSamplerandChannel(uint32_t animationIndex, int32_t accessorIndex, uint32_t timescaleAccessorIndex, int32_t boneNodeIndex, std::string targetPath);

		// Simplify adding a vector of data to the buffer
		// Not meant to be interleaved data, will be sequential
		// Assumes only 1 buffer exists
		// bufferViewIndex can be -1 to create new bufferview
		//int32_t addBufferData(const char* inData, uint32_t byteStride, uint32_t count, Accessor::Type dataType, Accessor::ComponentType componentType, int32_t bufferViewIndex);
		//int32_t addBufferData(const std::vector<char*> inData, uint32_t byteStride, uint32_t count, Accessor::Type dataType, Accessor::ComponentType componentType, int32_t bufferViewIndex);
		int32_t addBufferData(const std::vector<float>& inData, int32_t bufferViewIndex);
		int32_t addBufferData(const std::vector<glm::vec3>& inData, int32_t bufferViewIndex);
		int32_t addBufferData(const std::vector<glm::quat>& inData, int32_t bufferViewIndex);

		void createBufferAndScene();
		void findChildrenOfBone(const std::vector<BoneTreeNodeExternal>& boneTree, uint16_t boneIndex, std::vector<int32_t>& out);
		void modifyAsset();
	};
}