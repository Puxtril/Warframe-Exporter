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

		The move constructor used to contruct Internal structs.
		Once the Internal header is contructed, the External header should no longer be used.
		The data held within these structs is usually quite large, so we don't want to accidentally copy the data.

		TODO: Implement move constructors for rest of structs
		*/

		#ifndef HeaderExternal

		struct BoneTreeNodeExternal
		{
		private:
			std::string name;
			uint32_t parentIndex;

		public:
			BoneTreeNodeExternal() : name(), parentIndex(0) {}

			const std::string& getName() const { return name; }
			uint32_t getParentIndex() const { return parentIndex; }

			void setName(std::string inData) { name = inData; }
			void setParentIndex(uint32_t inData) { parentIndex = inData; }
		};

		struct MeshInfoExternal
		{
		private:
			std::string name;
			glm::vec4 vector1;
			glm::vec4 vector2;
			std::array<uint32_t, 5> faceLODOffsets;
			std::array<uint32_t, 5> faceLODCounts;

		public:
			MeshInfoExternal() = default;

			const std::string& getName() const { return name; }
			const glm::vec4& getVec1() const { return vector1; }
			const glm::vec4& getVec2() const { return vector2; }
			const std::array<uint32_t, 5>& getLODOffsets() const { return faceLODOffsets; }
			const std::array<uint32_t, 5>& getLODCounts() const { return faceLODCounts; }

			void setName(std::string inData) { name = inData; }
			glm::vec4& getVec1Ptr() { return vector1; }
			glm::vec4& getVec2Ptr() { return vector2; }
			std::array<uint32_t, 5>& getLODOffsetsPtr() { return faceLODOffsets; }
			std::array<uint32_t, 5>& getLODCountsPtr() { return faceLODCounts; }
		};

		struct PhysXMesh
		{
		private:
			uint32_t typeEnum;
			glm::vec4 vector1;
			glm::vec4 vector2;
			uint32_t dataLength;

		public:
			PhysXMesh() = default;

			uint32_t getTypeEnum() const { return typeEnum; }
			const glm::vec4& getVec1() const { return vector1; }
			const glm::vec4& getVec2() const { return vector2; }
			uint32_t getDataLength() const { return dataLength; }

			void setTypeEnum(uint32_t inData) { typeEnum = inData; }
			glm::vec4& getVec1Ptr() { return vector1; }
			glm::vec4& getVec2Ptr() { return vector2; }
			void setDataLength(uint32_t inData) { dataLength = inData; }
		};

		struct ModelHeaderExternal
		{
		private:
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

		public:
			ModelHeaderExternal() = default;
			ModelHeaderExternal(const ModelHeaderExternal&) = delete;
			ModelHeaderExternal& operator=(const ModelHeaderExternal& other) = delete;

			const std::vector<std::string>& getWeightedBoneNames() const { return weightedBoneNames; }
			const std::vector<BoneTreeNodeExternal>& getBoneTree() const { return boneTree; }
			uint32_t getVertexCount() const { return vertexCount; }
			uint32_t getBoneCount() const { return boneCount; }
			uint32_t getFaceCount() const { return faceCount; }
			uint32_t getMorphCount() const { return morphCount; }
			uint32_t getBodySkipLen1() const { return bodySkipLen1; }
			uint32_t getBodySkipLen2() const { return bodySkipLen2; }
			const std::vector<MeshInfoExternal>& getMeshInfos() const { return meshInfos; }
			const std::vector<std::vector<uint32_t>>& getBoneMaps() const { return boneMaps; }
			const std::vector<PhysXMesh>& getPhysXMeshes() const { return physXMeshes; }
			const std::vector<std::string>& getErrorMsgs() const { return errorMsgs; }

			void setWeightedBoneNames(std::vector<std::string>& inData) { weightedBoneNames = inData; }
			void setBoneTree(std::vector<BoneTreeNodeExternal>& inData) { boneTree = inData; }
			void setVertexCount(uint32_t inData) { vertexCount = inData; }
			void setBoneCount(uint32_t inData) { boneCount = inData; }
			void setFaceCount(uint32_t inData) { faceCount = inData; }
			void setMorphCount(uint32_t inData) { morphCount = inData; }
			void setBodySkipLen1(uint32_t inData) { bodySkipLen1 = inData; }
			void setBodySkipLen2(uint32_t inData) { bodySkipLen2 = inData; }
			void setMeshInfos(std::vector<MeshInfoExternal>& inData) { meshInfos = inData; }
			void setBoneMaps(std::vector<std::vector<uint32_t>>& inData) { boneMaps = inData; }
			void setPhysXMeshes(std::vector<PhysXMesh>& inData) { physXMeshes = inData; }
			void setErrorMsgs(std::vector<std::string>& inData) { errorMsgs = inData; }
		};

		#endif

		#ifndef HeaderInternal

		struct BoneTreeNodeInternal
		{
		private:
			std::string name;
			int32_t parentIndex;
			glm::quat rotation;
			glm::vec3 position;
			glm::mat4 reverseBind;

		public:
			BoneTreeNodeInternal() : name(std::string()), parentIndex(-1), rotation(glm::quat()), position(glm::vec3()), reverseBind(glm::mat4()) {};

			const std::string& getName() const { return name; }
			int32_t getParentIndex() const { return parentIndex; }
			const glm::quat& getRotation() const { return rotation; }
			const glm::vec3& getPosition() const { return position; }
			const glm::mat4& getReverseBind() const { return reverseBind; }

			void setName(std::string inData) { name = inData; }
			void setParentIndex(int32_t inData) { parentIndex = inData; }
			void setRotation(glm::quat& inData) { rotation = std::move(inData); }
			void setPosition(glm::vec3& inData) { position = std::move(inData); }
			void setReverseBind(glm::mat4& inData) { reverseBind = std::move(inData); }
		};

		struct MeshInfoInternal
		{
		private:
			std::string name;
			std::array<uint32_t, 5> faceLODOffsets;
			std::array<uint32_t, 5> faceLODCounts;
			std::string matName;

		public:
			MeshInfoInternal() = default;

			const std::string& getName() const { return name; }
			const std::array<uint32_t, 5>& getLODOffsets() const { return faceLODOffsets; }
			const std::array<uint32_t, 5>& getLODCounts() const { return faceLODCounts; }
			const std::string getMaterialName() const { return matName; }

			void setName(std::string inData) { name = inData; }
			std::array<uint32_t, 5>& getLODOffsetsPtr() { return faceLODOffsets; }
			std::array<uint32_t, 5>& getLODCountsPtr() { return faceLODCounts; }
			void setMaterialName(std::string inData) { matName = inData; }
		};

		struct ModelHeaderInternal
		{
		private:
			std::vector<size_t> weightedBones;
			std::vector<BoneTreeNodeInternal> boneTree;
			uint32_t vertexCount;
			uint32_t boneCount;
			uint32_t faceCount;
			uint32_t morphCount;
			std::vector<MeshInfoInternal> meshInfos;
			std::vector<std::string> errorMsgs;
			glm::vec3 modelScale;

		public:
			ModelHeaderInternal() = default;
			ModelHeaderInternal(const ModelHeaderInternal&) = delete;
			ModelHeaderInternal& operator=(const ModelHeaderInternal& other) = delete;

			ModelHeaderInternal(const ModelHeaderExternal& other) = delete;

			const std::vector<size_t>& getWeightedBones() const { return weightedBones; }
			const std::vector<BoneTreeNodeInternal>& getBoneTree() const { return boneTree; }
			uint32_t getVertexCount() const { return vertexCount; }
			uint32_t getBoneCount() const { return boneCount; }
			uint32_t getFaceCount() const { return faceCount; }
			uint32_t getMorphCount() const { return morphCount; }
			const std::vector<MeshInfoInternal>& getMeshInfos() const { return meshInfos; }
			const std::vector<std::string>& getErrorMsgs() const { return errorMsgs; }
			const glm::vec3& getModelScale() const { return modelScale; }

			void setWeightedBones(std::vector<size_t> inData) { weightedBones = inData; }
			void setBoneTree(std::vector<BoneTreeNodeInternal> inData) { boneTree = inData; }
			void setVertexCount(uint32_t inData) { vertexCount = inData; }
			void setBoneCount(uint32_t inData) { boneCount = inData; }
			void setFaceCount(uint32_t inData) { faceCount = inData; }
			void setMorphCount(uint32_t inData) { morphCount = inData; }
			void setMeshInfos(std::vector<MeshInfoInternal> inData) { meshInfos = inData; }
			void setErrorMsgs(std::vector<std::string> inData) { errorMsgs = inData; }
			glm::vec3& getModelScalePtr() { return modelScale; }
		};

		#endif

		#ifndef Body

		struct ModelBodyExternal
		{
		private:
			std::vector<glm::mat4x4> reverseBinds;
			std::vector<glm::quat> boneRotations;
			std::vector<glm::vec3> bonePositions;
			std::vector<uint16_t> indices;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> UV1;
			std::vector<glm::vec2> UV2;
			std::vector<glm::u8vec4> boneIndices;
			std::vector<glm::vec4> boneWeights;

		public:
			ModelBodyExternal() = default;
			ModelBodyExternal(const ModelBodyExternal& other) = delete;
			ModelBodyExternal& operator=(const ModelBodyExternal& other) = delete;

			const std::vector<glm::mat4x4>& getReverseBinds() const { return reverseBinds; }
			const std::vector<glm::quat>& getBoneRotations() const { return boneRotations; }
			const std::vector<glm::vec3>& getBonePositions() const { return bonePositions; }
			const std::vector<uint16_t>& getIndices() const { return indices; }
			const std::vector<glm::vec3>& getPositions() const { return positions; }
			const std::vector<glm::vec2>& getUV1() const { return UV1; }
			const std::vector<glm::vec2>& getUV2() const { return UV2; }
			const std::vector<glm::u8vec4>& getBoneIndices() const { return boneIndices; }
			const std::vector<glm::vec4>& getBoneWeights() const { return boneWeights; }

			std::vector<glm::mat4x4>& getReverseBindsPtr() { return reverseBinds; }
			std::vector<glm::quat>& getBoneRotationsPtr() { return boneRotations; }
			std::vector<glm::vec3>& getBonePositionsPtr() { return bonePositions; }
			std::vector<uint16_t>& getIndexPtr() { return indices; }
			std::vector<glm::vec3>& getPosPtr() { return positions; }
			std::vector<glm::vec2>& getUV1Ptr() { return UV1; }
			std::vector<glm::vec2>& getUV2Ptr() { return UV2; }
			std::vector<glm::u8vec4>& getBoneIndicesPtr() { return boneIndices; }
			std::vector<glm::vec4>& getBoneWeightsPtr() { return boneWeights; }
		};

		struct ModelBodyInternal
		{
		private:
			std::vector<uint16_t> indices;
			std::vector<glm::vec3> positions;
			std::vector<glm::vec2> UV1;
			std::vector<glm::vec2> UV2;
			std::vector<glm::u16vec4> boneIndices;
			std::vector<glm::vec4> boneWeights;

		public:
			static const int positionLen = 12;
			static const int colorLen = 4;
			static const int UVLen = 8;
			static const int boneIndexLen = 8;
			static const int boneWeightLen = 16;

			ModelBodyInternal() = default;
			ModelBodyInternal(const ModelBodyInternal& other) = delete;
			ModelBodyInternal& operator=(const ModelBodyInternal& other) = delete;

			int32_t vertexSizeRigged() const
			{
				return positionLen + (UVLen * 2) + boneIndexLen + boneWeightLen;
			}

			int32_t vertexSizeStatic() const
			{
				return positionLen + (UVLen * 2);
			}

			const std::vector<uint16_t>& getIndices() const { return indices; }
			const std::vector<glm::vec3>& getPositions() const { return positions; }
			const std::vector<glm::vec2>& getUV1() const { return UV1; }
			const std::vector<glm::vec2>& getUV2() const { return UV2; }
			const std::vector<glm::u16vec4>& getBoneIndices() const { return boneIndices; }
			const std::vector<glm::vec4>& getBoneWeights() const { return boneWeights; }

			void setIndices(std::vector<uint16_t>& inData) { indices = std::move(inData); }
			void setPositions(std::vector<glm::vec3>& inData) { positions = std::move(inData); }
			void setUV1(std::vector<glm::vec2>& inData) { UV1 = std::move(inData); }
			void setUV2(std::vector<glm::vec2>& inData) { UV2 = std::move(inData); }
			void setBoneWeights(std::vector<glm::vec4>& inData) { boneWeights = std::move(inData); }
			std::vector<glm::u16vec4>& getBoneIndexPtr() { return boneIndices; }
		};

		#endif
	}
}
