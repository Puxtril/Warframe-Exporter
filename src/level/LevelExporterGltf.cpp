#include "level/LevelExporterGltf.h"

using namespace WarframeExporter::Level;

void
LevelExporterGltf::addModelData(Document& gltfDoc, const WarframeExporter::Model::ModelHeaderInternal& header, const WarframeExporter::Model::ModelBodyInternal& bodyInt, const WarframeExporter::Model::ModelBodyExternal& bodyExt, const LevelObjectInternal& levelObj)
{
	int32_t nodeCountInit = static_cast<int32_t>(gltfDoc.nodes.size());
	Model::ModelExporterGltf::addModelData(gltfDoc, header, bodyInt, bodyExt);
	int32_t nodeCountAfter = static_cast<int32_t>(gltfDoc.nodes.size());

	std::vector<int32_t> meshNodeIndices = _getMeshNodeIndices(gltfDoc, nodeCountInit, nodeCountAfter);
	_addLevelInformation(gltfDoc, levelObj, meshNodeIndices);
}

void
LevelExporterGltf::_addLevelInformation(Document& gltfDoc, const LevelObjectInternal& levelObj, std::vector<int32_t> meshIndices)
{
	for (size_t x = 0; x < meshIndices.size(); x++)
	{
		int32_t meshIndex = meshIndices[x];
		Node& curNode = gltfDoc.nodes[meshIndex];

		glm::mat4 matrix(1.0f);
		matrix = glm::translate(matrix, {levelObj.pos.x / 2.0f, levelObj.pos.y / 2.0f, -levelObj.pos.z / 2.0f});
		matrix *= glm::scale(matrix, {1.0, 1.0, -1.0});
		matrix *= glm::toMat4(levelObj.rot);
		matrix = glm::scale(matrix, {levelObj.scale, levelObj.scale, levelObj.scale});

		curNode.matrix = {
			matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
			matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
			matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
			matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3],
		};

		Mesh& curMesh = gltfDoc.meshes[curNode.mesh];
		curMesh.extensionsAndExtras["extras"]["MeshPath"] = levelObj.meshPath;
		curMesh.extensionsAndExtras["extras"]["Scale"] = levelObj.scale;
		for (const auto& x : levelObj.attributes.items())
			curMesh.extensionsAndExtras["extras"][x.key()] = x.value();
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