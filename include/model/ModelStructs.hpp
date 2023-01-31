#pragma once

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtx/quaternion.hpp"

#include <string>
#include <cstdint>
#include <vector>
#include <array>

namespace WarframeExporter
{
	namespace Model
	{
		/*
		External structs represent the data read directly from the file
		Internal structs represent the external data formatted in a more compatable way
		  for further processing.
		*/

		struct BoneTreeNodeExternal
		{
			std::string name;
			uint32_t parentIndex;
		};

		struct MeshInfoExternal
		{
			std::string name;
			glm::vec4 vector1;
			glm::vec4 vector2;
			std::array<uint32_t, 5> faceLODOffsets;
			std::array<uint32_t, 5> faceLODCounts;
		};

		struct PhysXMesh
		{
			uint32_t typeEnum;
			glm::vec4 vector1;
			glm::vec4 vector2;
			uint32_t dataLength;
		};

		struct ModelHeaderExternal
		{
			std::vector<std::string> weightedBoneNames;
			std::vector<BoneTreeNodeExternal> boneTree;
			uint32_t vertexCount;
			uint32_t boneCount;
			uint32_t faceCount;
			uint32_t morphCount;
			uint32_t bodySkipLen1;
			uint32_t bodySkipLen2;
			std::vector<MeshInfoExternal> meshInfos;
			std::vector<std::vector<uint32_t>> boneMaps;
			std::vector<PhysXMesh> physXMeshes;
			std::vector<std::string> errorMsgs;
		};

		struct BoneTreeNodeInternal
		{
			std::string name;
			int32_t parentIndex;
			glm::quat rotation;
			glm::vec3 position;
			glm::mat4 reverseBind;
		};

		struct MeshInfoInternal
		{
			std::string name;
			std::array<uint32_t, 5> faceLODOffsets;
			std::array<uint32_t, 5> faceLODCounts;
			std::string matName;
		};

		struct ModelHeaderInternal
		{
			std::vector<size_t> weightedBones;
			std::vector<BoneTreeNodeInternal> boneTree;
			uint32_t vertexCount;
			uint32_t boneCount;
			uint32_t faceCount;
			uint32_t morphCount;
			std::vector<MeshInfoInternal> meshInfos;
			std::vector<std::string> errorMsgs;
			glm::vec3 modelScale;
		};

		struct ModelBodyExternal
		{
			std::vector<glm::mat4x4> reverseBinds;
			std::vector<glm::quat> boneRotations;
			std::vector<glm::vec3> bonePositions;
			std::vector<uint16_t> indices;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> UV1;
			std::vector<glm::vec2> UV2;
			std::vector<glm::u8vec4> boneIndices;
			std::vector<glm::vec4> boneWeights;
		};

		struct ModelBodyInternal
		{
			std::vector<uint16_t> indices;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> UV1;
			std::vector<glm::vec2> UV2;
			std::vector<glm::u16vec4> boneIndices;
			std::vector<glm::vec4> boneWeights;
			
			static const int positionLen = 12;
			static const int colorLen = 4;
			static const int UVLen = 8;
			static const int boneIndexLen = 8;
			static const int boneWeightLen = 16;

			int32_t vertexSizeRigged() const
			{
				return positionLen + (UVLen * 2) + boneIndexLen + boneWeightLen;
			}

			int32_t vertexSizeStatic() const
			{
				return positionLen + (UVLen * 2);
			}
		};
	}
}
