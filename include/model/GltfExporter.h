#pragma once

#include "ModelStructs.hpp"
#include "fx/gltf.h"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <unordered_map>
#include <unordered_set>
#include <cstdint>
#include <algorithm>
#include <filesystem>
#include <iostream>

using namespace fx::gltf;

namespace WarframeExporter
{
	namespace Model
	{
		class GltfModel
		{
			Document m_document;
			inline static const std::string m_generatorName = "Puxtrils Warframe Model Exporter";
			inline static const std::string m_copyright = "DIGITAL EXTREMES Ltd.";

		public:
			GltfModel();

			void addModelData(const ModelHeaderInternal& header, const ModelBodyInternal& body);
			void save(const std::string& outPathStr);

		private:
			void print_exception(const std::exception& e, int level = 0);

			void addModelDataRigged(const ModelHeaderInternal& header, const ModelBodyInternal& body);
			void addModelDataStatic(const ModelHeaderInternal& header, const ModelBodyInternal& body);

			void createSceneWithModelNodes(const std::vector<int32_t>& meshes, int32_t skinIndex);

			// Returns index of root bone in tree
			// Garuntees same order as boneTree
			int32_t createBones(const std::vector<BoneTreeNodeInternal>& boneTree);
			// Returns index of new skin
			int32_t createSkin(const std::vector<size_t>& weightedIndices, size_t totalBoneCount, const std::string& skinName, int32_t rootBoneIndex, int32_t inverseBindMatricesIndex);
			// Add Inverse Bind Matrices to the buffer and return the accessor index
			int32_t addInverseBindMatrices(const std::vector<BoneTreeNodeInternal>& boneTree, const std::vector<size_t>& weightedIndices);

			// Returns index of new Mesh
			std::vector<int32_t> createMeshes(const std::vector<MeshInfoInternal>& meshInfos, Attributes attrs, int32_t indicesBuffViewIndex);

			int32_t findOrCreateMaterial(const std::string& materialName);
	
			// Returns Attributes for Primetives
			Attributes addVertexDataRigged(const ModelBodyInternal& body, size_t vertCount);
			Attributes addVertexDataStatic(const ModelBodyInternal& body, size_t vertCount);
	
			// Returns index of the created BufferView
			int32_t addIndexData(const std::vector<uint16_t>& body);

			void modifyAsset();
			void createBuffer();
			void checkAndFixBufferAllignment();
			std::vector<float> findMaxVec3(const std::vector<glm::vec3>& body);
			std::vector<float> findMinVec3(const std::vector<glm::vec3>& body);
			void findChildrenOfBone(const std::vector<BoneTreeNodeInternal>& boneTree, uint16_t boneIndex, std::vector<int32_t>& out);
		};
	}
}