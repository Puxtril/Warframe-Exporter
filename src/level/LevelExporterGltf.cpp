#include "level/LevelExporterGltf.h"

using namespace WarframeExporter::Level;

std::vector<Mesh>
LevelExporterGltf::addModel(Document& gltfDoc, const WarframeExporter::Model::ModelHeaderInternal& header, const WarframeExporter::Model::ModelBodyInternal& bodyInt, const WarframeExporter::Model::ModelBodyExternal& bodyExt, const LevelObjectInternal& levelObj)
{
	WarframeExporter::Model::ModelExporterGltf::_modifyAsset(gltfDoc);

	Attributes vertsAttrs = WarframeExporter::Model::ModelExporterGltf::_addVertexData(gltfDoc, bodyInt, bodyExt, header.vertexCount);
	std::vector<int32_t> indicesAccessors = WarframeExporter::Model::ModelExporterGltf::_addIndexData(gltfDoc, bodyInt.indices, header.meshInfos);

	std::vector<Mesh> meshes(indicesAccessors.size());
	for (size_t meshIndex = 0; meshIndex < indicesAccessors.size(); meshIndex++)
	{
		meshes[meshIndex] = WarframeExporter::Model::ModelExporterGltf::_createMesh(gltfDoc, vertsAttrs, indicesAccessors[meshIndex]);
		WarframeExporter::Model::ModelExporterGltf::_addModelExtraInformation(gltfDoc, meshes[meshIndex], header);
	}

	return meshes;
}

void
LevelExporterGltf::addModelInstance(Document& gltfDoc, const LevelObjectInternal& levelObj, const std::vector<Mesh>& meshes)
{
	if (gltfDoc.scenes.size() == 0)
	{
		Scene scene;
		gltfDoc.scenes.resize(1);
		gltfDoc.scene = 0;
	}
	Scene& scene = gltfDoc.scenes[0];

	for (size_t i = 0; i < meshes.size(); i++)
	{
		const Mesh& curMesh = meshes[i];
		
		Node node;
		glm::mat4 matrix(1.0f);
		matrix = glm::translate(matrix, {levelObj.pos.x / 2.0f, levelObj.pos.y / 2.0f, -levelObj.pos.z / 2.0f});
		matrix *= glm::scale(matrix, {1.0, 1.0, -1.0});
		matrix *= glm::toMat4(levelObj.rot);
		matrix = glm::scale(matrix, {levelObj.scale, levelObj.scale, levelObj.scale});

		node.matrix = {
			matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
			matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
			matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
			matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3],
		};

		int curMeshIndex = static_cast<int>(gltfDoc.meshes.size());
		gltfDoc.meshes.push_back(curMesh);
		node.mesh = curMeshIndex;
		int curNodeIndex = static_cast<int>(gltfDoc.nodes.size());
		gltfDoc.nodes.push_back(node);
		scene.nodes.push_back(curNodeIndex);

		Mesh& insertedMesh = gltfDoc.meshes.back();

		insertedMesh.name = levelObj.objName;

		insertedMesh.extensionsAndExtras["extras"]["MeshPath"] = levelObj.meshPath;
		insertedMesh.extensionsAndExtras["extras"]["Scale"] = levelObj.scale;
		for (const auto& x : levelObj.attributes.items())
			insertedMesh.extensionsAndExtras["extras"][x.key()] = x.value();
	}
}

std::vector<int32_t>
LevelExporterGltf::_getMeshNodeIndices(Document& gltfDoc, int32_t startIndex, int32_t endIndex)
{
	std::vector<int32_t> indices;
	for (int32_t x = startIndex; x < endIndex; x++)
	{
		if (gltfDoc.nodes[x].mesh != -1)
			indices.push_back(x);
	}
	return indices;
}