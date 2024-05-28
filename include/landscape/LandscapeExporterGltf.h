#pragma once

#include "model/ModelExporterGltf.h"
#include "landscape/LandscapeStructs.h"

namespace WarframeExporter::Landscape
{
    class LandscapeExporterGltf : public Model::ModelExporterGltf
    {
    public:
        void addLandscapeChunks(const LandscapeInternal& landscape);

    protected:
        Mesh addLandscapeChunk(const Physx::HeightFieldMeshSplit& mesh);
        Attributes addLandscapeVertices(const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer);
        int32_t addPositions(const Physx::HeightFieldMeshSplit& mesh, Buffer& buffer);

        // Returns index of Accessors
        int32_t addLandscapeIndices(const std::vector<uint32_t>& indices, Buffer& buffer);

        // Materials are currently unknown, so we're just naming them "Material <index>"
        void resizeMaterials(size_t newSize);

        void addExtraAttributes(const std::vector<std::vector<std::string>>& materialPathArrays, Mesh& mesh, size_t chunkIndex);
    };
}