#include "landscape/LandscapeExporterGltf.h"

using namespace WarframeExporter::Landscape;

void
LandscapeExporterGltf::addLandscapeChunks(const LandscapeInternal& landscape)
{
    for (size_t i = 0; i < landscape.chunks.size(); i++)
    {
        int32_t curMeshIndex = static_cast<int32_t>(m_document.meshes.size());

        if (landscape.chunks[i].verts.size() == 4)
            continue;

        Mesh gltfMesh = addLandscapeChunk(landscape.chunks[i]);
        gltfMesh.name = "Landscape " + std::to_string(i);

        addExtraAttributes(landscape.materialPathArrays, gltfMesh, i);
        
        m_document.meshes.push_back(gltfMesh);
        m_document.scenes.resize(1);
        Scene& scene = m_document.scenes[0];

        Node curNode;        
        std::memcpy(&curNode.matrix[0], &landscape.transforms[i][0], 16 * sizeof(float));
        curNode.mesh = curMeshIndex;
        int curNodeIndex = static_cast<int>(m_document.nodes.size());
        m_document.nodes.push_back(curNode);
        scene.nodes.push_back(curNodeIndex);
    }

}

Mesh
LandscapeExporterGltf::addLandscapeChunk(const Physx::HeightFieldMeshSplit& mesh)
{
    m_document.buffers.resize(1);
    Buffer& buf = m_document.buffers[0];

    Attributes vertAttributes = addLandscapeVertices(mesh, buf);

    Mesh gltfMesh;
    resizeMaterials(mesh.indexArrays.size());

    for (uint32_t materialIndex = 0; materialIndex < mesh.indexArrays.size(); materialIndex++)
    {
        if (mesh.indexArrays[materialIndex].size() == 0)
            continue;

        int32_t indicesIndex = addLandscapeIndices(mesh.indexArrays[materialIndex], buf);

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
LandscapeExporterGltf::addLandscapeVertices(const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer)
{
    Attributes attrs;
	attrs["POSITION"] = addPositions(mesh, buffer);
    attrs["TEXCOORD_0"] = generateAndAddUVs(mesh, buffer);
    return attrs;
}

int32_t
LandscapeExporterGltf::addPositions(const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer)
{
    const uint32_t vertexDataSize = static_cast<uint32_t>(mesh.verts.size()) * sizeof(std::array<float, 3>);
    uint32_t curSize = static_cast<uint32_t>(buffer.data.size());
    uint32_t newSize = curSize + vertexDataSize;
    buffer.data.resize(newSize);
    buffer.byteLength = newSize;
    memcpy(buffer.data.data() + curSize, mesh.verts.data(), vertexDataSize);
    
    BufferView bufView;
	int32_t bufViewIndex = (int32_t)m_document.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = curSize;
	bufView.byteLength = vertexDataSize;
	bufView.byteStride = sizeof(float) * 3;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	m_document.bufferViews.push_back(bufView);

    Accessor posAcc;
	int32_t posAccIndex = (int32_t)m_document.accessors.size();
	posAcc.bufferView = bufViewIndex;
	posAcc.byteOffset = 0;
	posAcc.count = static_cast<uint32_t>(mesh.verts.size());
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::Float;
	m_document.accessors.push_back(posAcc);

    return posAccIndex;
}

int32_t
LandscapeExporterGltf::generateAndAddUVs(const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer)
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
	int32_t bufViewIndex = (int32_t)m_document.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = curSize;
	bufView.byteLength = uvDataSize;
	bufView.byteStride = sizeof(float) * 2;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	m_document.bufferViews.push_back(bufView);

    Accessor uvAcc;
	int32_t uvAccIndex = (int32_t)m_document.accessors.size();
	uvAcc.bufferView = bufViewIndex;
	uvAcc.byteOffset = 0;
	uvAcc.count = static_cast<uint32_t>(mesh.verts.size());
	uvAcc.type = Accessor::Type::Vec2;
	uvAcc.componentType = Accessor::ComponentType::Float;
	m_document.accessors.push_back(uvAcc);

    return uvAccIndex;
}

int32_t
LandscapeExporterGltf::addLandscapeIndices(const std::vector<uint32_t>& indices, Buffer& buffer)
{
    const uint32_t indexDataSize = static_cast<uint32_t>(indices.size()) * sizeof(uint32_t);
	uint32_t curSize = static_cast<uint32_t>(buffer.data.size());
    uint32_t newSize = curSize + indexDataSize;
    buffer.data.resize(newSize);
	buffer.byteLength = newSize;
	std::memcpy(buffer.data.data() + curSize, indices.data(), indexDataSize);

    BufferView bufView;
	int32_t bufViewIndex2 = (int32_t)m_document.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = curSize;
	bufView.byteLength = indexDataSize;
	bufView.target = BufferView::TargetType::ElementArrayBuffer;
	m_document.bufferViews.push_back(bufView);

    Accessor accessor;
    int32_t accessorIndex = static_cast<int32_t>(m_document.accessors.size());
    accessor.bufferView = bufViewIndex2;
    accessor.byteOffset = 0;
    accessor.count = static_cast<uint32_t>(indices.size());
    accessor.type = Accessor::Type::Scalar;
    accessor.componentType = Accessor::ComponentType::UnsignedInt;
    m_document.accessors.push_back(accessor);

    return accessorIndex;
}

void
LandscapeExporterGltf::resizeMaterials(size_t newSize)
{
    if (m_document.materials.size() >= newSize)
        return;

    for (size_t newMatIndex = m_document.materials.size(); newMatIndex < (size_t)newSize; newMatIndex++)
    {
        Material newMat;
        newMat.name = "Material " + std::to_string(newMatIndex);
        newMat.extensionsAndExtras["extras"]["id"] = newMatIndex;
        m_document.materials.push_back(std::move(newMat));
    }
}

void
LandscapeExporterGltf::addExtraAttributes(const std::vector<std::vector<std::string>>& materialPathArrays, Mesh& mesh, size_t chunkIndex)
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