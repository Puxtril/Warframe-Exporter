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

namespace WarframeExporter::Model
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
		std::array<uint32_t, 5> faceLODVertexOffsets;
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
		// After Warframe Update 40 (Vallis Undermind) vertices are split between B and F cache
		uint32_t vertexCountB;
		uint32_t faceCountB;
		glm::vec4 ensmallening1;
		glm::vec4 ensmallening2;
		// 2 "free use" variables for skipping data
		uint32_t bodySkipLen1;
		uint32_t bodySkipLen2;
		std::vector<MeshInfoExternal> meshInfos;
		std::vector<std::vector<uint32_t>> boneMaps;
		std::vector<std::string> materialPaths; // Only present on "level" meshes
		std::vector<PhysXMesh> physXMeshes;
		std::vector<std::string> errorMsgs;
	};

	struct BoneTreeNodeInternal
	{
		std::string name;
		int parentIndex;
		glm::quat rotation;
		glm::vec3 position;
		glm::mat4 reverseBind;
	};

	struct MeshInfoInternal
	{
		std::string name;
		std::array<int, 5> faceLODOffsets;
		std::array<int, 5> faceLODCounts;
		std::string matName;
	};

	struct ModelHeaderInternal
	{
		std::vector<int32_t> weightedBones;
		std::vector<BoneTreeNodeInternal> boneTree;
		int vertexCount;
		int boneCount;
		int faceCount;
		int morphCount;
		std::vector<MeshInfoInternal> meshInfos;
		std::vector<std::string> errorMsgs;
		glm::vec4 modelScale;
	};

	struct ModelBodyExternal
	{
		std::vector<glm::mat4x4> reverseBinds;
		std::vector<glm::quat> boneRotations;
		std::vector<glm::vec3> bonePositions;
		std::vector<uint16_t> indices;
		std::vector<glm::vec4> positions;
		std::vector<glm::vec2> UV1;
		std::vector<glm::vec2> UV2;
		std::vector<uint8_t> AO;
		std::vector<glm::u8vec4> normals;
		std::vector<glm::u8vec3> tangents;
		std::vector<std::vector<glm::u8vec4>> colors;
		std::vector<glm::u8vec4> boneIndices;
		std::vector<glm::vec4> boneWeights;
	};

	struct ModelBodyInternal
	{
		std::vector<uint16_t> indices;
		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> UV1;
		std::vector<glm::vec2> UV2;
		std::vector<uint8_t> AO;
		std::vector<std::vector<glm::u8vec4>> colors;
		std::vector<glm::u16vec4> boneIndices;
		std::vector<glm::vec4> boneWeights;

		constexpr int32_t positionTypeSize() const { return sizeof(positions[0]); }
		constexpr int32_t UVTypeSize() const { return sizeof(UV1[0]); }
		constexpr int32_t colorTypeSize() const { return sizeof(colors[0][0]); }
		constexpr int32_t AOTypeSize() const { return sizeof(AO[0]); }
		constexpr int32_t boneIndexTypeSize() const { return sizeof(boneIndices[0]); }
		constexpr int32_t boneWeightTypeSize() const { return sizeof(boneWeights[0]); }

		int32_t vertexSizeRigged() const
		{
			return static_cast<int32_t>(positionTypeSize() + (UVTypeSize() * 2) + (colorTypeSize() * colors.size()) + boneIndexTypeSize() + boneWeightTypeSize());
		}

		int32_t vertexSizeStatic() const
		{
			return static_cast<int32_t>(positionTypeSize() + (UVTypeSize() * 2) + (colorTypeSize() * colors.size()));
		}
	};
}
