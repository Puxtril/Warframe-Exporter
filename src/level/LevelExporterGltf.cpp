#include "level/LevelExporterGltf.h"

using namespace WarframeExporter::Level;

void
LevelExporterGltf::addModelData(const WarframeExporter::Model::ModelHeaderInternal& header, const WarframeExporter::Model::ModelBodyInternal& body, const LevelObjectInternal& levelObj)
{
	int nodeCountInit = m_document.nodes.size();
	ModelExporterGltf::addModelData(header, body);
	int nodeCountAfter = m_document.nodes.size();

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

		Mesh& curMesh = m_document.meshes[curNode.mesh];
		curMesh.extensionsAndExtras["extras"]["MeshPath"] = levelObj.meshPath;
		curMesh.extensionsAndExtras["extras"]["Scale"] = levelObj.scale;
		for (const auto& x : levelObj.attributes)
			curMesh.extensionsAndExtras["extras"][std::get<0>(x)] = std::get<1>(x);

		if (levelObj.materials.size() == meshIndices.size())
		{
			int32_t newMatIndex = createMaterial(std::string(levelObj.materials[x]));
			curMesh.primitives[0].material = newMatIndex;
			Material& curMaterial = m_document.materials[curMesh.primitives[0].material];
		}
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

int32_t
LevelExporterGltf::createMaterial(const std::string& materialPath)
{
	std::string materialName = materialPath;
	size_t lastSlashOffset = materialPath.find_last_of("/");
	if (lastSlashOffset != std::string::npos)
		materialName = materialPath.substr(lastSlashOffset + 1);

	int32_t matIndex = static_cast<int32_t>(m_document.materials.size());
	Material mat;
	mat.name = materialName;
	mat.doubleSided = true;
	mat.extensionsAndExtras["extras"]["FullPath"] = materialPath;
	m_document.materials.push_back(mat);

	return matIndex;
}