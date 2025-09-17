#include "level/LevelExporterGltf.h"

using namespace WarframeExporter::Level;

void
LevelExporterGltf::addModel(Document& gltfDoc, const WarframeExporter::Model::ModelHeaderInternal& header, const WarframeExporter::Model::ModelBodyInternal& bodyInt, const WarframeExporter::Model::ModelBodyExternal& bodyExt, const LevelObjectInternal& levelObj)
{
	WarframeExporter::Model::ModelExporterGltf::_modifyAsset(gltfDoc);

	Attributes vertsAttrs = WarframeExporter::Model::ModelExporterGltf::_addVertexData(gltfDoc, bodyInt, bodyExt, header.vertexCount);
	std::vector<int32_t> indicesAccessors = WarframeExporter::Model::ModelExporterGltf::_addIndexData(gltfDoc, bodyInt.indices, header.meshInfos);

	for (size_t meshIndex = 0; meshIndex < indicesAccessors.size(); meshIndex++)
	{
		Mesh createdMesh = WarframeExporter::Model::ModelExporterGltf::_createMesh(gltfDoc, vertsAttrs, indicesAccessors[meshIndex], header.meshInfos[meshIndex].matName, header.meshInfos[meshIndex].name);
		WarframeExporter::Model::ModelExporterGltf::_addModelExtraInformation(gltfDoc, createdMesh, header);
		_addModelToScene(gltfDoc, levelObj, createdMesh);
	}
}

void
LevelExporterGltf::_addModelToScene(Document& gltfDoc, const LevelObjectInternal& levelObj, Mesh& mesh)
{
	if (gltfDoc.scenes.size() == 0)
	{
		Scene scene;
		gltfDoc.scenes.resize(1);
		gltfDoc.scene = 0;
	}
	Scene& scene = gltfDoc.scenes[0];
		
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
	gltfDoc.meshes.push_back(mesh);
	node.mesh = curMeshIndex;
	int curNodeIndex = static_cast<int>(gltfDoc.nodes.size());
	gltfDoc.nodes.push_back(node);
	scene.nodes.push_back(curNodeIndex);

	Mesh& insertedMesh = gltfDoc.meshes.back();

	insertedMesh.name = levelObj.objName;

	insertedMesh.extensionsAndExtras["extras"]["MeshPath"] = levelObj.meshPath;
	insertedMesh.extensionsAndExtras["extras"]["Scale"] = levelObj.scale;

	// Before adding the remaining JSON values, we need to check for valid ASCII
	for (const auto& x : levelObj.attributes.items())
	{
		bool canAdd = true;
		for (const char& curChar : x.key())
		{
			if (curChar < 0 || curChar > 126)
			{
				canAdd = false;
				break;
			}
		}
		if (canAdd)
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