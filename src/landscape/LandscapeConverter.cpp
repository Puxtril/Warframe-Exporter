#include "landscape/LandscapeConverter.h"

using namespace WarframeExporter::Landscape;

std::tuple<int, int>
LandscapeConverter::fixGridCount(const LandscapeHeaderExternal& externalHeader)
{
    if (externalHeader.rowCount + externalHeader.columnCount == externalHeader.chunks.size())
        return {externalHeader.rowCount, externalHeader.columnCount};

    if (externalHeader.rowCount != externalHeader.columnCount)
    {
        WarframeExporter::Logger::getInstance().warn(spdlog::fmt_lib::format("Invalid landscape grid {}x{} with {} chunks", externalHeader.rowCount, externalHeader.columnCount, externalHeader.chunks.size()));
        return {externalHeader.rowCount, externalHeader.columnCount};
    }

    int sqroot = lround(sqrt(externalHeader.chunks.size()));
    return {sqroot, sqroot};
}

void
LandscapeConverter::positionChunks(const LandscapeHeaderExternal& externalHeader, const std::vector<LandscapeBodyChunkExternal>& externalChunks, LandscapeInternal& internal)
{
    std::tuple<int, int> grid = fixGridCount(externalHeader);

    for (int iRow = 0; iRow < std::get<0>(grid); iRow++)
    {
        for (int iColumn = 0; iColumn < std::get<1>(grid); iColumn++)
        {
            int chunkIndex = iRow + iColumn;
            const glm::vec3& scale = externalHeader.chunks[chunkIndex].scale;
            internal.positions.push_back({iRow * scale.x, 0.0, iColumn * scale.z});
        }
    }
}

void
LandscapeConverter::scaleChunks(Physx::HeightFieldMesh& mesh, const LandscapeHeaderChunkExternal& extHeaderChunk, const LandscapeBodyChunkExternal& extBodyChunk)
{
    for (size_t i = 0; i < mesh.vertexPositions.size(); i++)
    {
        std::array<float, 3>& curVert = mesh.vertexPositions[i];

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
        glm::mat4 translate = glm::translate(glm::mat4(1.0F), glm::vec3(landscape.positions[i][0], 0.0, landscape.positions[i][2]));
        glm::mat4 rotate = glm::rotate(glm::mat4(1.0F), glm::radians(90.0F), glm::vec3(0.0F, 1.0F, 0.0F));
        landscape.transforms.push_back(rotate * translate);
    }
}