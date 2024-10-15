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

		curNode.translation = std::array<float, 3>{levelObj.pos.x, levelObj.pos.y, levelObj.pos.z};

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