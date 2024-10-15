#pragma once

#include "ExporterLogger.h"
#include "ModelStructs.hpp"
#include "fx/gltf.h"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <cstdint>
#include <filesystem>

using namespace fx::gltf;

namespace WarframeExporter::Model::ModelExporterGltf
{
	static const std::string m_generatorName = "Puxtrils Warframe Model Exporter";
	static const std::string m_copyright = "DIGITAL EXTREMES Ltd.";

	void addModelData(Document& gltfDoc, const ModelHeaderInternal& header, const ModelBodyInternal& bodyInt, const ModelBodyExternal& bodyExt);
	void save(Document& gltfDoc, const std::filesystem::path& outPath);

	void _print_exception(const std::exception& e, int level = 0);

	void _addModelsToScene(Document& gltfDoc, const std::vector<int32_t>& meshes, int32_t skinIndex = -1);

	// Returns index of root bone in tree
	// Garuntees same order as boneTree
	int32_t _createBones(Document& gltfDoc, const std::vector<BoneTreeNodeInternal>& boneTree);
	// Returns index of new skin
	int32_t _createSkin(Document& gltfDoc, const std::vector<int32_t>& weightedIndices, int totalBoneCount, const std::string& skinName, int32_t rootBoneIndex, int32_t inverseBindMatricesIndex);
	// Add Inverse Bind Matrices to the buffer and return the accessor index
	int32_t _addInverseBindMatrices(Document& gltfDoc, const std::vector<BoneTreeNodeInternal>& boneTree, const std::vector<int32_t>& weightedIndices);

	// Returns index of new Mesh
	std::vector<int32_t> _createMeshes(Document& gltfDoc, const std::vector<MeshInfoInternal>& meshInfos, Attributes attrs, int32_t indicesBuffViewIndex);

	int32_t _findOrCreateMaterial(Document& gltfDoc, const std::string& materialPath);

	// Returns Attributes for Primetives
	Attributes _addVertexData(Document& gltfDoc, const ModelBodyInternal& bodyInt, const ModelBodyExternal& bodyExt, int vertCount);

	// Returns index of the created BufferView
	int32_t _addIndexData(Document& gltfDoc, const std::vector<uint16_t>& body);

	void _modifyAsset(Document& gltfDoc);

	Buffer& _getBuffer(Document& gltfDoc);

	std::vector<float> _findMaxVec3(const std::vector<glm::vec3>& body);
	std::vector<float> _findMinVec3(const std::vector<glm::vec3>& body);
	void _findChildrenOfBone(const std::vector<BoneTreeNodeInternal>& boneTree, uint16_t boneIndex, std::vector<int32_t>& out);
}