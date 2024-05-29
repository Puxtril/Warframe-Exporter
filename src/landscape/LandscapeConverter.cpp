#include "landscape/LandscapeConverter.h"

using namespace WarframeExporter::Landscape;

void
LandscapeConverter::positionChunks(const LandscapeHeaderExternal& externalHeader, const std::vector<LandscapeBodyChunkExternal>& externalChunks, LandscapeInternal& internal)
{
    for (unsigned int iRow = 0; iRow < externalHeader.rowCount; iRow++)
    {
        for (unsigned int iColumn = 0; iColumn < externalHeader.columnCount; iColumn++)
        {
            unsigned int chunkIndex = iRow + iColumn;
            const glm::vec3& scale = externalHeader.chunks[chunkIndex].scale;
            internal.positions.push_back({iRow * scale.x, 0.0, iColumn * scale.z});
        }
    }
}

void
LandscapeConverter::scaleChunks(Physx::HeightFieldMeshSplit& mesh, const LandscapeHeaderChunkExternal& extHeaderChunk, const LandscapeBodyChunkExternal& extBodyChunk)
{
    for (size_t i = 0; i < mesh.verts.size(); i++)
    {
        std::array<float, 3>& curVert = mesh.verts[i];

        // Bring into 0.0 - 1.0 range using Physx data
        curVert[0] = (curVert[0] - extBodyChunk.header.minBounds[0]) / (extBodyChunk.header.maxBounds[0] - extBodyChunk.header.minBounds[0]);
        curVert[1] /= 256;
        curVert[2] = (curVert[2] - extBodyChunk.header.minBounds[2]) / (extBodyChunk.header.maxBounds[2] - extBodyChunk.header.minBounds[2]);

        // Bring into Warframe's scale
        curVert[0] *= extHeaderChunk.scale.x;
        curVert[2] *= extHeaderChunk.scale.z;
    }
}

void
LandscapeConverter::addTransforms(LandscapeInternal& landscape)
{
    for (size_t i = 0; i < landscape.chunks.size(); i++)
    {
        const Physx::HeightFieldMeshSplit& curMesh = landscape.chunks[i];

        glm::mat4 translate = glm::translate(glm::mat4(1.0F), glm::vec3(landscape.positions[i][0], 0.0, landscape.positions[i][2]));
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0F), glm::radians(90.0F), glm::vec3(0.0F, 1.0F, 0.0F));
        landscape.transforms.push_back(rotate * translate);
    }
}