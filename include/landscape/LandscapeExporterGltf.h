#pragma once

#include "model/ModelExporterGltf.h"
#include "landscape/LandscapeStructs.h"

#include <cstring>

namespace WarframeExporter::Landscape::LandscapeExporterGltf
{
    void addLandscapeChunks(Document& gltfDoc, const LandscapeInternal& landscape);

    Mesh _addLandscapeChunk(Document& gltfDoc, const Physx::HeightFieldMesh& mesh);
    Attributes _addLandscapeVertices(Document& gltfDoc, const Physx::HeightFieldMesh& mesh, Buffer& buffer);
    int32_t _addPositions(Document& gltfDoc, const Physx::HeightFieldMesh& mesh, Buffer& buffer);
    int32_t _generateAndAddUVs(Document& gltfDoc, const Physx::HeightFieldMesh& mesh, Buffer& buffer);
    int32_t _addVertexColors(Document& gltfDoc, const Physx::HeightFieldMesh& mesh, Buffer& buffer);

    Buffer& _getBuffer(Document& gltfDoc);
    void _addExtraAttributes(Document& gltfDoc, const std::vector<std::vector<std::string>>& materialPathArrays, Mesh& mesh, size_t chunkIndex);
}