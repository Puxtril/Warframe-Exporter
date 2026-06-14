#pragma once

#include "model/ModelExporterGltf.h"
#include "landscape/LandscapeStructs.h"

#include <cstring>

namespace WarframeExporter::Landscape::LandscapeExporterGltf
{
    void addLandscapeChunks(Document& gltfDoc, const LandscapeInternal& landscape, const glm::vec3& pos);

    Mesh _addLandscapeChunk(Document& gltfDoc, const LandscapeChunkInternal& chunk);
    Attributes _addLandscapeVertices(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer);
    int32_t _addPositions(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer);
    int32_t _addIndices(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer);
    int32_t _generateAndAddUVs(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer);
    int32_t _addVertexColors(Document& gltfDoc, const LandscapeChunkInternal& chunk, Buffer& buffer);

    Buffer& _getBuffer(Document& gltfDoc);
    void _addExtraAttributes(Document& gltfDoc, const std::vector<std::vector<std::string>>& materialPathArrays, Mesh& mesh, size_t chunkIndex);
}