#include "levelstatic/LevelStaticExporterGltf.h"

std::vector<Mesh>
WarframeExporter::LevelStatic::ExporterGltf::addModel(
    Document& gltfDoc,
    const WarframeExporter::Model::ModelHeaderInternal& header,
    const WarframeExporter::Model::ModelBodyInternal& modelBodyInt,
    const WarframeExporter::Model::ModelBodyExternal& modelBodyExt
)
{
    WarframeExporter::Model::ModelExporterGltf::_modifyAsset(gltfDoc);

	Attributes vertsAttrs = WarframeExporter::Model::ModelExporterGltf::_addVertexData(gltfDoc, modelBodyInt, modelBodyExt, header.vertexCount);
	std::vector<int32_t> indicesAccessors = WarframeExporter::Model::ModelExporterGltf::_addIndexData(gltfDoc, modelBodyInt.indices, header.meshInfos);

    std::vector<Mesh> meshes(indicesAccessors.size());
	for (size_t meshIndex = 0; meshIndex < indicesAccessors.size(); meshIndex++)
	{
		meshes[meshIndex] = WarframeExporter::Model::ModelExporterGltf::_createMesh(gltfDoc, vertsAttrs, indicesAccessors[meshIndex], header.meshInfos[meshIndex].matName, header.meshInfos[meshIndex].name);
		WarframeExporter::Model::ModelExporterGltf::_addModelExtraInformation(gltfDoc, meshes[meshIndex], header);
	}

	return meshes;
}

void
WarframeExporter::LevelStatic::ExporterGltf::addModelInstance(
    Document& gltfDoc,
    const LevelStaticHeaderExternal& levelStaticHeaderExt,
    const LevelStaticObjectExternal& levelObj,
    std::vector<Mesh> models
)
{
    if (gltfDoc.scenes.size() == 0)
    {
        Scene scene;
        gltfDoc.scenes.resize(1);
        gltfDoc.scene = 0;
    }
    Scene& scene = gltfDoc.scenes[0];

    for (size_t i = 0; i < models.size(); i++)
	{
        const Mesh& curMesh = models[i];
		
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

		insertedMesh.extensionsAndExtras["extras"]["Mesh"] = levelStaticHeaderExt.modelPaths[levelObj.modelIndex];
        if (levelObj.materialIndex < levelStaticHeaderExt.materialPaths.size())
            insertedMesh.extensionsAndExtras["extras"]["Material"] = levelStaticHeaderExt.materialPaths[levelObj.materialIndex];
        if (levelObj.naturalIndex < levelStaticHeaderExt.naturalPaths.size())
            insertedMesh.extensionsAndExtras["extras"]["Natural"] = levelStaticHeaderExt.naturalPaths[levelObj.naturalIndex];
		insertedMesh.extensionsAndExtras["extras"]["Scale"] = levelObj.scale;
    }
}