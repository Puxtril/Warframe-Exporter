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

        curNode.translation = std::array<float, 3>{levelObj.pos.x, levelObj.pos.y, levelObj.pos.z};

        Mesh& curMesh = gltfDoc.meshes[curNode.mesh];
		curMesh.extensionsAndExtras["extras"]["Mesh"] = levelStaticHeaderExt.modelPaths[levelObj.modelIndex];
        if (levelObj.materialIndex < levelStaticHeaderExt.materialPaths.size())
            curMesh.extensionsAndExtras["extras"]["Material"] = levelStaticHeaderExt.materialPaths[levelObj.materialIndex];
        if (levelObj.naturalIndex < levelStaticHeaderExt.naturalPaths.size())
            curMesh.extensionsAndExtras["extras"]["Natural"] = levelStaticHeaderExt.naturalPaths[levelObj.naturalIndex];
		curMesh.extensionsAndExtras["extras"]["Scale"] = levelObj.scale;
    }
}