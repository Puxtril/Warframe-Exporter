#include "level/LevelExporterGltf.h"

using namespace WarframeExporter::Level;

void
LevelExporterGltf::addModelData(const WarframeExporter::Model::ModelHeaderInternal& header, const WarframeExporter::Model::ModelBodyInternal& body, const LevelObjectInternal& levelObj)
{
	int32_t nodeCountInit = static_cast<int32_t>(m_document.nodes.size());
	ModelExporterGltf::addModelData(header, body);
	int32_t nodeCountAfter = static_cast<int32_t>(m_document.nodes.size());

	std::vector<int32_t> meshNodeIndices = getMeshNodeIndices(nodeCountInit, nodeCountAfter);
	addLevelInformation(levelObj, meshNodeIndices);
}

void
LevelExporterGltf::addLevelInformation(const LevelObjectInternal& levelObj, std::vector<int32_t> meshIndices)
{
	for (size_t x = 0; x < meshIndices.size(); x++)
	{
		int32_t meshIndex = meshIndices[x];
		Node& curNode = m_document.nodes[meshIndex];

		curNode.translation = std::array<float, 3>{levelObj.pos.x, levelObj.pos.y, levelObj.pos.z};

		Mesh& curMesh = m_document.meshes[curNode.mesh];
		curMesh.extensionsAndExtras["extras"]["MeshPath"] = levelObj.meshPath;
		curMesh.extensionsAndExtras["extras"]["Scale"] = levelObj.scale;
		for (const auto& x : levelObj.attributes)
			curMesh.extensionsAndExtras["extras"][std::get<0>(x)] = std::get<1>(x);
	}
}

std::vector<int32_t>
LevelExporterGltf::getMeshNodeIndices(int32_t startIndex, int32_t endIndex)
{
	std::vector<int32_t> indices;
	for (int32_t x = startIndex; x < endIndex; x++)
	{
		if (m_document.nodes[x].mesh != -1)
			indices.push_back(x);
	}
	return indices;
}