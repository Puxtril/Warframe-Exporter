#pragma once

#include "model/ModelExporterGltf.h"
#include "landscape/LandscapeStructs.h"

#include <cstring>

namespace WarframeExporter::Landscape::LandscapeExporterGltf
{
    void addLandscapeChunks(Document& gltfDoc, const LandscapeInternal& landscape);

    Mesh _addLandscapeChunk(Document& gltfDoc, const Physx::HeightFieldMeshSplit& mesh);
    Attributes _addLandscapeVertices(Document& gltfDoc, const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer);
    int32_t _addPositions(Document& gltfDoc, const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer);
    int32_t _generateAndAddUVs(Document& gltfDoc, const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer);

    // Returns index of Accessors
    int32_t _addLandscapeIndices(Document& gltfDoc, const std::vector<uint32_t>& indices, Buffer& buffer);

    // Materials are currently unknown, so we're just naming them "Material <index>"
    void _resizeMaterials(Document& gltfDoc, size_t newSize);

    void _addExtraAttributes(Document& gltfDoc, const std::vector<std::vector<std::string>>& materialPathArrays, Mesh& mesh, size_t chunkIndex);
}