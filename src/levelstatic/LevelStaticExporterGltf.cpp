#include "levelstatic/LevelStaticExporterGltf.h"

void
WarframeExporter::LevelStatic::ExporterGltf::addModelData(
    Document& gltfDoc,
    const WarframeExporter::Model::ModelHeaderInternal& header,
    const WarframeExporter::Model::ModelBodyInternal& modelBodyInt,
    const WarframeExporter::Model::ModelBodyExternal& modelBodyExt,
    const LevelStaticHeaderExternal& levelStaticHeaderExt,
    const LevelStaticObjectExternal& levelStaticObj
)
{
    int32_t nodeCountInit = static_cast<int32_t>(gltfDoc.nodes.size());
	Model::ModelExporterGltf::addModelData(gltfDoc, header, modelBodyInt, modelBodyExt);
	int32_t nodeCountAfter = static_cast<int32_t>(gltfDoc.nodes.size());

	std::vector<int32_t> meshNodeIndices = WarframeExporter::Level::LevelExporterGltf::_getMeshNodeIndices(gltfDoc, nodeCountInit, nodeCountAfter);
    _addLevelInformation(gltfDoc, levelStaticHeaderExt, levelStaticObj, meshNodeIndices);
}

void
WarframeExporter::LevelStatic::ExporterGltf::_addLevelInformation(Document& gltfDoc, const LevelStaticHeaderExternal& levelStaticHeaderExt, const LevelStaticObjectExternal& levelObj, std::vector<int32_t> meshIndices)
{
    for (size_t x = 0; x < meshIndices.size(); x++)
	{
        int32_t meshIndex = meshIndices[x];
		Node& curNode = gltfDoc.nodes[meshIndex];

        glm::mat4 matrix(1.0f);
        matrix = glm::translate(matrix, {levelObj.pos.x / 2.0f, levelObj.pos.y / 2.0f, levelObj.pos.z / 2.0f});
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
		curMesh.extensionsAndExtras["extras"]["Mesh"] = levelStaticHeaderExt.modelPaths[levelObj.modelIndex];
        if (levelObj.materialIndex < levelStaticHeaderExt.materialPaths.size())
            curMesh.extensionsAndExtras["extras"]["Material"] = levelStaticHeaderExt.materialPaths[levelObj.materialIndex];
        if (levelObj.naturalIndex < levelStaticHeaderExt.naturalPaths.size())
            curMesh.extensionsAndExtras["extras"]["Natural"] = levelStaticHeaderExt.naturalPaths[levelObj.naturalIndex];
		curMesh.extensionsAndExtras["extras"]["Scale"] = levelObj.scale;
    }
}