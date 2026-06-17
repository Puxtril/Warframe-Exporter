#include "landscape/LandscapeExporterGltf.h"

using namespace WarframeExporter::Landscape;

void
LandscapeExporterGltf::addLandscapeChunks(Document& gltfDoc, const LandscapeInternal& landscape, const glm::vec3& pos)
{
    int chunkOffsetX = (landscape.srcChunkCountX - landscape.chunkCountX) / 2;
    int chunkOffsetY = (landscape.srcChunkCountY - landscape.chunkCountY) / 2;

    for (size_t i = 0; i < landscape.chunks.size(); i++)
    {
        int32_t curMeshIndex = static_cast<int32_t>(gltfDoc.meshes.size());

        if (landscape.chunks[i].body.vertexPositions.size() <= 12)
            continue;

        Mesh gltfMesh = _addLandscapeChunk(gltfDoc, landscape.chunks[i]);

        int nameX = (i % landscape.chunkCountX) + chunkOffsetX;
        int nameY = (landscape.chunkCountY - 1 - (i / landscape.chunkCountX)) + chunkOffsetY;
        gltfMesh.name = "Landscape T" + std::to_string(nameX) + "u" + std::to_string(nameY);

        _addExtraAttributes(gltfDoc, landscape.materialPathArrays, gltfMesh, i);
        
        gltfDoc.meshes.push_back(gltfMesh);
        gltfDoc.scenes.resize(1);
        Scene& scene = gltfDoc.scenes[0];

        Node curNode;
        glm::mat4 transforms = glm::translate(landscape.transforms[i], {pos.z, pos.y, pos.x});
        std::memcpy(&curNode.matrix[0], &transforms[0], 16 * sizeof(float));
        
        curNode.mesh = curMeshIndex;
        int curNodeIndex = static_cast<int>(gltfDoc.nodes.size());
        gltfDoc.nodes.push_back(curNode);
        scene.nodes.push_back(curNodeIndex);
    }
}

Mesh
LandscapeExporterGltf::_addLandscapeChunk(Document& gltfDoc, const LandscapeChunkInternal& chunk)
{
    Buffer& buf = _getBuffer(gltfDoc);

    Attributes vertAttributes = _addLandscapeVertices(gltfDoc, chunk, buf);

    Primitive primitive;
    primitive.indices = _addIndices(gltfDoc, chunk, buf);
    primitive.mode = Primitive::Mode::Triangles;
    primitive.attributes = vertAttributes;

    Mesh gltfMesh;
    gltfMesh.primitives.push_back(std::move(primitive));

    return gltfMesh;
}

Attributes
LandscapeExporterGltf::_addLandscapeVertices(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer)
{
    Attributes attrs;
	attrs["POSITION"] = _addPositions(gltfDoc, chunk, buffer);
    attrs["TEXCOORD_0"] = _generateAndAddUVs(gltfDoc, chunk, buffer);
    attrs["COLOR_0"] = _addVertexColors(gltfDoc, chunk, buffer);
    return attrs;
}

int32_t
LandscapeExporterGltf::_addPositions(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer)
{
    const uint32_t vertexDataSize = static_cast<uint32_t>(chunk.body.vertexPositions.size()) * sizeof(std::array<float, 3>);
    uint32_t curSize = static_cast<uint32_t>(buffer.data.size());
    uint32_t newSize = curSize + vertexDataSize;
    buffer.data.resize(newSize);
    buffer.byteLength = newSize;
    memcpy(buffer.data.data() + curSize, chunk.body.vertexPositions.data(), vertexDataSize);
    
    BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = gltfDoc.buffers.size() - 1;
	bufView.byteOffset = curSize;
	bufView.byteLength = vertexDataSize;
	bufView.byteStride = sizeof(float) * 3;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

    Accessor posAcc;
	int32_t posAccIndex = (int32_t)gltfDoc.accessors.size();
	posAcc.bufferView = bufViewIndex;
	posAcc.byteOffset = 0;
	posAcc.count = static_cast<uint32_t>(chunk.body.vertexPositions.size());
	posAcc.type = Accessor::Type::Vec3;
	posAcc.componentType = Accessor::ComponentType::Float;
	gltfDoc.accessors.push_back(posAcc);

    return posAccIndex;
}

int32_t 
LandscapeExporterGltf::_addIndices(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer)
{
    Buffer& buf = _getBuffer(gltfDoc);

	uint32_t indexSize = (uint32_t)chunk.body.indices.size() * sizeof(uint32_t);
	uint32_t startOffset = buf.byteLength;

	buf.data.resize(startOffset + indexSize);
	std::memcpy(&buf.data[startOffset], chunk.body.indices.data(), indexSize);
	buf.byteLength += indexSize;

	BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = gltfDoc.buffers.size() - 1;
	bufView.byteOffset = startOffset;
	bufView.byteLength = indexSize;
	bufView.target = BufferView::TargetType::ElementArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

    Accessor curAcc;
    int32_t curAccIndex = static_cast<int32_t>(gltfDoc.accessors.size());
    curAcc.bufferView = bufViewIndex;
    curAcc.byteOffset = 0;
    curAcc.count = chunk.body.indices.size();
    curAcc.type = Accessor::Type::Scalar;
    curAcc.componentType = Accessor::ComponentType::UnsignedInt;
    gltfDoc.accessors.push_back(curAcc);
    
	return curAccIndex;
}

int32_t
LandscapeExporterGltf::_generateAndAddUVs(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer)
{
    const uint32_t uvDataSize = static_cast<uint32_t>(chunk.body.vertexPositions.size()) * (sizeof(uint16_t) * 2);
    uint32_t curSize = static_cast<uint32_t>(buffer.data.size());
    uint32_t newSize = curSize + uvDataSize;
    buffer.data.resize(newSize);
    buffer.byteLength = newSize;

    for (size_t i = 0; i < chunk.body.vertexPositions.size(); i++)
    {
        const uint16_t v = (chunk.body.vertexPositions[i][2] / chunk.scale.x) * UINT16_MAX;
        const uint16_t u = (1.0F - chunk.body.vertexPositions[i][0] / chunk.scale.y) * UINT16_MAX;
        memcpy(buffer.data.data() + curSize + (i * 2 * 2), &v, 2);
        memcpy(buffer.data.data() + curSize + (i * 2 * 2) + 2, &u, 2);
    }

    BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = gltfDoc.buffers.size() - 1;
	bufView.byteOffset = curSize;
	bufView.byteLength = uvDataSize;
	bufView.byteStride = sizeof(uint16_t) * 2;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

    Accessor uvAcc;
	int32_t uvAccIndex = (int32_t)gltfDoc.accessors.size();
	uvAcc.bufferView = bufViewIndex;
	uvAcc.byteOffset = 0;
	uvAcc.count = static_cast<uint32_t>(chunk.body.vertexPositions.size());
	uvAcc.type = Accessor::Type::Vec2;
	uvAcc.componentType = Accessor::ComponentType::UnsignedShort;
    uvAcc.normalized = true;
    uvAcc.max = {UINT16_MAX, UINT16_MAX, UINT16_MAX};
    uvAcc.min = {0, 0, 0};
	gltfDoc.accessors.push_back(uvAcc);

    return uvAccIndex;
}

int32_t
LandscapeExporterGltf::_addVertexColors(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer)
{
    const uint32_t colorDataSize = static_cast<uint32_t>(chunk.body.materials.size() * 3);
    uint32_t curSize = static_cast<uint32_t>(buffer.data.size());
    uint32_t newSize = curSize + colorDataSize;
    buffer.data.resize(newSize);
    buffer.byteLength = newSize;

    for (size_t i = 0; i < chunk.body.materials.size(); i++)
    {
        std::fill_n(buffer.data.data() + curSize + (i * 3), 3, chunk.body.materials[i]);
    }

    BufferView bufView;
	int32_t bufViewIndex = (int32_t)gltfDoc.bufferViews.size();
	bufView.buffer = gltfDoc.buffers.size() - 1;
	bufView.byteOffset = curSize;
	bufView.byteLength = colorDataSize;
	bufView.byteStride = 3;
	bufView.target = BufferView::TargetType::ArrayBuffer;
	gltfDoc.bufferViews.push_back(bufView);

    Accessor colAcc;
	int32_t colAccIndex = (int32_t)gltfDoc.accessors.size();
	colAcc.bufferView = bufViewIndex;
	colAcc.byteOffset = 0;
	colAcc.count = static_cast<uint32_t>(chunk.body.materials.size());
	colAcc.type = Accessor::Type::Vec3;
    colAcc.normalized = true;
	colAcc.componentType = Accessor::ComponentType::UnsignedByte;
	gltfDoc.accessors.push_back(colAcc);

    return colAccIndex;
}

Buffer&
LandscapeExporterGltf::_getBuffer(Document& gltfDoc)
{
	if (gltfDoc.buffers.size() == 0)
	{
		Buffer newBuf;
		newBuf.name = "Model Data 1";
		newBuf.byteLength = 0;
		gltfDoc.buffers.push_back(newBuf);
	}

	return gltfDoc.buffers.back();
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