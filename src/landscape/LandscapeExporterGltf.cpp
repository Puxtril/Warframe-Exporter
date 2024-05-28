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
        curNode.translation = {landscape.positions[i][0], 0.0, landscape.positions[i][2]};
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
    //attrs["COLOR_0"] = addColors(mesh, buffer);

    return attrs;
}

int32_t
LandscapeExporterGltf::addPositions(const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer)
{
    const size_t vertexDataSize = mesh.verts.size() * sizeof(std::array<float, 3>);
    uint32_t curSize = buffer.data.size();
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
	posAcc.count = mesh.verts.size();
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::Float;
	//posAcc.max = findMaxVec3(mesh.verts);
	//posAcc.min = findMinVec3(mesh.verts);
	m_document.accessors.push_back(posAcc);

    return posAccIndex;
}

int32_t
LandscapeExporterGltf::addColors(const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer)
{
    /*
    const size_t colorDataSize = mesh.materialIds.size() * sizeof(uint8_t);
    uint32_t curSize = buffer.data.size();
    uint32_t newSize = curSize + (colorDataSize * 3);
    buffer.data.resize(newSize);
    buffer.byteLength = newSize;
    for (size_t i = 0; i < mesh.materialIds.size(); i++)
    {
        if (mesh.materialIds[i] != 127)
            std::cout << i << " " << (int)mesh.materialIds[i] << " " << std::endl;
        memcpy(buffer.data.data() + curSize + (i * 3), &mesh.materialIds[i], 1);
        memcpy(buffer.data.data() + curSize + (i * 3) + 1, &mesh.materialIds[i], 1);
        memcpy(buffer.data.data() + curSize + (i * 3) + 2, &mesh.materialIds[i], 1);
    }
    
    BufferView bufView;
	int32_t bufViewIndex = (int32_t)m_document.bufferViews.size();
	bufView.buffer = 0;
	bufView.byteOffset = curSize;
	bufView.byteLength = colorDataSize * 3;
	bufView.byteStride = 3;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	m_document.bufferViews.push_back(bufView);

    Accessor posAcc;
	int32_t posAccIndex = (int32_t)m_document.accessors.size();
	posAcc.bufferView = bufViewIndex;
	posAcc.byteOffset = 0;
	posAcc.count = mesh.materialIds.size();
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::UnsignedByte;
	//posAcc.max = findMaxVec3(mesh.verts);
	//posAcc.min = findMinVec3(mesh.verts);
	m_document.accessors.push_back(posAcc);

    return posAccIndex;
    */
}


int32_t
LandscapeExporterGltf::addLandscapeIndices(const std::vector<uint32_t>& indices, Buffer& buffer)
{
    const uint32_t indexDataSize = indices.size() * sizeof(uint32_t);
	uint32_t curSize = buffer.data.size();
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
    accessor.count = indices.size();
    accessor.type = Accessor::Type::Scalar;
    accessor.componentType = Accessor::ComponentType::UnsignedInt;
    m_document.accessors.push_back(accessor);

    return accessorIndex;
}

void
LandscapeExporterGltf::resizeMaterials(uint32_t newSize)
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