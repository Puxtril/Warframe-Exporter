#include "landscape/LandscapeExporterGltf.h"

using namespace WarframeExporter::Landscape;

void
LandscapeExporterGltf::addLandscapeChunks(Document& gltfDoc, const LandscapeInternal& landscape)
{
    for (size_t i = 0; i < landscape.chunks.size(); i++)
    {
        int32_t curMeshIndex = static_cast<int32_t>(gltfDoc.meshes.size());

        if (landscape.chunks[i].verts.size() == 4)
            continue;

        Mesh gltfMesh = _addLandscapeChunk(gltfDoc, landscape.chunks[i]);
        gltfMesh.name = "Landscape " + std::to_string(i);

        _addExtraAttributes(gltfDoc, landscape.materialPathArrays, gltfMesh, i);
        
        gltfDoc.meshes.push_back(gltfMesh);
        gltfDoc.scenes.resize(1);
        Scene& scene = gltfDoc.scenes[0];

        Node curNode;        
        std::memcpy(&curNode.matrix[0], &landscape.transforms[i][0], 16 * sizeof(float));
        curNode.mesh = curMeshIndex;
        int curNodeIndex = static_cast<int>(gltfDoc.nodes.size());
        gltfDoc.nodes.push_back(curNode);
        scene.nodes.push_back(curNodeIndex);
    }
}

Mesh
LandscapeExporterGltf::_addLandscapeChunk(Document& gltfDoc, const Physx::HeightFieldMeshSplit& mesh)
{
    gltfDoc.buffers.resize(1);
    Buffer& buf = gltfDoc.buffers[0];

    Attributes vertAttributes = _addLandscapeVertices(gltfDoc, mesh, buf);

    Mesh gltfMesh;
    _resizeMaterials(gltfDoc, mesh.indexArrays.size());

    for (uint32_t materialIndex = 0; materialIndex < mesh.indexArrays.size(); materialIndex++)
    {
        if (mesh.indexArrays[materialIndex].size() == 0)
            continue;

        int32_t indicesIndex = _addLandscapeIndices(gltfDoc, mesh.indexArrays[materialIndex], buf);

        Primitive primitive;
        primitive.indices = indicesIndex;
        primitive.mode = Primitive::Mode::Triangles;
        primitive.attributes = vertAttributes;
        primitive.material = materialIndex;

        gltfMesh.primitives.push_back(std::move(primitive));
    }

    return gltfMesh;
}

Attributes
LandscapeExporterGltf::_addLandscapeVertices(Document& gltfDoc, const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer)
{
    Attributes attrs;
	attrs["POSITION"] = _addPositions(gltfDoc, mesh, buffer);
    attrs["TEXCOORD_0"] = _generateAndAddUVs(gltfDoc, mesh, buffer);
    return attrs;
}

int32_t
LandscapeExporterGltf::_addPositions(Document& gltfDoc, const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer)
{
    const uint32_t vertexDataSize = static_cast<uint32_t>(mesh.verts.size()) * sizeof(std::array<float, 3>);
    uint32_t curSize = static_cast<uint32_t>(buffer.data.size());
    uint32_t newSize = curSize + vertexDataSize;
    buffer.data.resize(newSize);
    buffer.byteLength = newSize;
    memcpy(buffer.data.data() + curSize, mesh.verts.data(), vertexDataSize);
    
    BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = curSize;
	bufView.byteLength = vertexDataSize;
	bufView.byteStride = sizeof(float) * 3;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

    Accessor posAcc;
	int32_t posAccIndex = (int32_t)gltfDoc.accessors.size();
	posAcc.bufferView = bufViewIndex;
	posAcc.byteOffset = 0;
	posAcc.count = static_cast<uint32_t>(mesh.verts.size());
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(posAcc);

    return posAccIndex;
}

int32_t
LandscapeExporterGltf::_generateAndAddUVs(Document& gltfDoc, const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer)
{
    const uint32_t uvDataSize = static_cast<uint32_t>(mesh.verts.size()) * (sizeof(float) * 2);
    uint32_t curSize = static_cast<uint32_t>(buffer.data.size());
    uint32_t newSize = curSize + uvDataSize;
    buffer.data.resize(newSize);
    buffer.byteLength = newSize;

    uint32_t maxU = 0;
    uint32_t maxV = 0;
    for (size_t i = 0; i < mesh.verts.size(); i++)
    {
        if (mesh.verts[i][0] > maxU)
            maxU = mesh.verts[i][0];
        if (mesh.verts[i][2] > maxV)
            maxV = mesh.verts[i][2];
    }

    for (size_t i = 0; i < mesh.verts.size(); i++)
    {
        const float v = mesh.verts[i][2] / (float)maxV;
        const float u = 1.0F - mesh.verts[i][0] / (float)maxU;
        memcpy(buffer.data.data() + curSize + (i * 2 * 4), &v, 4);
        memcpy(buffer.data.data() + curSize + (i * 2 * 4) + 4, &u, 4);
    }
    
    BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = curSize;
	bufView.byteLength = uvDataSize;
	bufView.byteStride = sizeof(float) * 2;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

    Accessor uvAcc;
	int32_t uvAccIndex = (int32_t)gltfDoc.accessors.size();
	uvAcc.bufferView = bufViewIndex;
	uvAcc.byteOffset = 0;
	uvAcc.count = static_cast<uint32_t>(mesh.verts.size());
	uvAcc.type = Accessor::Type::Vec2;
	uvAcc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(uvAcc);

    return uvAccIndex;
}

int32_t
LandscapeExporterGltf::_addLandscapeIndices(Document& gltfDoc, const std::vector<uint32_t>& indices, Buffer& buffer)
{
    const uint32_t indexDataSize = static_cast<uint32_t>(indices.size()) * sizeof(uint32_t);
	uint32_t curSize = static_cast<uint32_t>(buffer.data.size());
    uint32_t newSize = curSize + indexDataSize;
    buffer.data.resize(newSize);
	buffer.byteLength = newSize;
	std::memcpy(buffer.data.data() + curSize, indices.data(), indexDataSize);

    BufferView bufView;
	int32_t bufViewIndex2 = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = curSize;
	bufView.byteLength = indexDataSize;
	bufView.target = BufferView::TargetType::ElementArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

    Accessor accessor;
    int32_t accessorIndex = static_cast<int32_t>(gltfDoc.accessors.size());
    accessor.bufferView = bufViewIndex2;
    accessor.byteOffset = 0;
    accessor.count = static_cast<uint32_t>(indices.size());
    accessor.type = Accessor::Type::Scalar;
    accessor.componentType = Accessor::ComponentType::UnsignedInt;
    gltfDoc.accessors.push_back(accessor);

    return accessorIndex;
}

void
LandscapeExporterGltf::_resizeMaterials(Document& gltfDoc, size_t newSize)
{
    if (gltfDoc.materials.size() >= newSize)
        return;

    for (size_t newMatIndex = gltfDoc.materials.size(); newMatIndex < (size_t)newSize; newMatIndex++)
    {
        Material newMat;
        newMat.name = "Material " + std::to_string(newMatIndex);
        newMat.extensionsAndExtras["extras"]["id"] = newMatIndex;
        gltfDoc.materials.push_back(std::move(newMat));
    }
}

void
LandscapeExporterGltf::_addExtraAttributes(Document& gltfDoc, const std::vector<std::vector<std::string>>& materialPathArrays, Mesh& mesh, size_t chunkIndex)
{
    mesh.extensionsAndExtras["extras"]["LCHM"] = materialPathArrays[0].size() > chunkIndex ? materialPathArrays[0][chunkIndex] : "";
    mesh.extensionsAndExtras["extras"]["LCMM1"] =  materialPathArrays[1].size() > chunkIndex ? materialPathArrays[1][chunkIndex] : "";
    mesh.extensionsAndExtras["extras"]["Empty1"] =  materialPathArrays[2].size() > chunkIndex ? materialPathArrays[2][chunkIndex] : "";
    mesh.extensionsAndExtras["extras"]["LNM"] =  materialPathArrays[3].size() > chunkIndex ? materialPathArrays[3][chunkIndex] : "";
    mesh.extensionsAndExtras["extras"]["LPM"] =  materialPathArrays[4].size() > chunkIndex ? materialPathArrays[4][chunkIndex] : "";
    mesh.extensionsAndExtras["extras"]["LCXM"] =  materialPathArrays[5].size() > chunkIndex ? materialPathArrays[5][chunkIndex] : "";
    mesh.extensionsAndExtras["extras"]["LCFM"] =  materialPathArrays[6].size() > chunkIndex ? materialPathArrays[6][chunkIndex] : "";
    mesh.extensionsAndExtras["extras"]["LCMM2"] =  materialPathArrays[7].size() > chunkIndex ? materialPathArrays[7][chunkIndex] : "";
    mesh.extensionsAndExtras["extras"]["Empty2"] =  materialPathArrays[8].size() > chunkIndex ? materialPathArrays[8][chunkIndex] : "";
}