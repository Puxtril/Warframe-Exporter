#include "landscape/LandscapeConverter.h"

using namespace WarframeExporter::Landscape;

std::tuple<int, int>
LandscapeConverter::fixGridCount(const LandscapeHeaderExternal& externalHeader)
{
    if (externalHeader.rowCount + externalHeader.columnCount == externalHeader.chunks.size())
        return {externalHeader.rowCount, externalHeader.columnCount};

    if (externalHeader.rowCount != externalHeader.columnCount)
    {
        if (externalHeader.rowCount * externalHeader.columnCount != externalHeader.chunks.size())
            throw unknown_format_error("Non-square grid with invalid chunk size: (" + std::to_string(externalHeader.rowCount) + "," + std::to_string(externalHeader.columnCount) + "): " + std::to_string(externalHeader.chunks.size()));
        
        return {externalHeader.rowCount, externalHeader.columnCount};
    }

    int sqroot = lround(sqrt(externalHeader.chunks.size()));
    return {sqroot, sqroot};
}

void
LandscapeConverter::positionChunks(const LandscapeHeaderExternal& externalHeader, const std::vector<LandscapeBodyChunkExternal>& externalChunks, LandscapeInternal& internal)
{
    std::tuple<int, int> grid = fixGridCount(externalHeader);
    internal.chunkCountY = std::get<0>(grid);
    internal.chunkCountX = std::get<1>(grid);

    for (int iRow = 0; iRow < internal.chunkCountY; iRow++)
    {
        for (int iColumn = 0; iColumn < internal.chunkCountX; iColumn++)
        {
            int chunkIndex = iRow + iColumn;
            const glm::vec3& scale = externalHeader.chunks[chunkIndex].scale;
            internal.positions.push_back({iRow * scale.x, 0.0, iColumn * scale.z});
        }
    }
}

void
LandscapeConverter::scaleChunks(Physx::HeightFieldIndexedMesh& mesh, const LandscapeHeaderChunkExternal& extHeaderChunk, const LandscapeBodyChunkExternal& extBodyChunk)
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

std::vector<LandscapeChunkInternal>
LandscapeConverter::convertToInternalMultithread(const LandscapeHeaderExternal& landscapeHeader, const std::vector<LandscapeBodyChunkExternal>& landscapeBody)
{
    const size_t chunkCount = landscapeBody.size();
    std::vector<LandscapeChunkInternal> outputChunks(chunkCount);

    const size_t threadCount = std::min(std::max((size_t)8, (size_t)std::thread::hardware_concurrency()), chunkCount);
    size_t runningThreadCount = 0;
    size_t remainingChunkCount = chunkCount;

    std::vector<bool> inputQueue(chunkCount, false);
    std::vector<std::thread> runningQueue(chunkCount);

    // Populate the output queue
    for (size_t i = 0; i < threadCount; i++)
    {
        runningQueue[i] = std::thread(LandscapeConverter::convertLandscapeTask, &landscapeBody[i], &landscapeHeader.chunks[i], &outputChunks, i);
        inputQueue[i] = true;
        runningThreadCount++;
        remainingChunkCount--;
    }

    // Parse output queue
    while (true)
    {
        // Join completed threads
        for (size_t i = 0; i < chunkCount; i++)
        {
            if (runningQueue[i].joinable() && outputChunks[i].header.sampleCount == landscapeBody[i].samples.size())
            {
                runningQueue[i].join();
                runningThreadCount--;
            }
        }

        // Create new threads
        for (size_t i = 0; i < chunkCount; i++)
        {
            if (runningThreadCount < threadCount)
            {
                if (!runningQueue[i].joinable() && !inputQueue[i])
                {
                    runningQueue[i] = std::thread(LandscapeConverter::convertLandscapeTask, &landscapeBody[i], &landscapeHeader.chunks[i], &outputChunks, i);
                    inputQueue[i] = true;
                    runningThreadCount++;
                    remainingChunkCount--;
                }
            }
        }

        // Done
        if (runningThreadCount == 0 && remainingChunkCount == 0)
            break;

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    return outputChunks;
}

void
LandscapeConverter::convertLandscapeTask(const LandscapeBodyChunkExternal* chunk, const LandscapeHeaderChunkExternal* eeChunkHeader, std::vector<LandscapeChunkInternal>* outputChunks, size_t outputChunkIndex)
{
    Physx::HeightFieldIndexedMesh mesh = Physx::HeightFieldReader::convertToIndexedMesh(chunk->header, chunk->samples);
    LandscapeConverter::scaleChunks(mesh, *eeChunkHeader, *chunk);
    (*outputChunks)[outputChunkIndex] = {chunk->header, mesh, eeChunkHeader->scale };
}