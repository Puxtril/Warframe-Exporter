#pragma once

#include "heightfield/Structs.h"
#include "landscape/LandscapeStructs.h"
#include "glm/gtx/quaternion.hpp"
#include "ExporterLogger.h"
#include "ExporterExceptions.h"

#include "heightfield/Reader.h"

#include <cmath>
#include <tuple>
#include <thread>
#include <chrono>

namespace WarframeExporter::Landscape
{
    class LandscapeConverter
    {
    public:
        // Sometime the row/column count doesn't add up to the chunk count.
        // Ex. There will be 25 chunks (5x5), but the file reads 9x9.
        // In this example, and from what I've seen, grids of square sizes can
        //   have incorrect sizes, but rectangular grids (1x2, 1x3, etc) will
        //   always be corrrect.
        // Thus, this function simply fixes square grids.
        static std::tuple<int, int> fixGridCount(const LandscapeHeaderExternal& externalHeader);

        static void positionChunks(const LandscapeHeaderExternal& externalHeader, const std::vector<LandscapeBodyChunkExternal>& externalChunks, LandscapeInternal& internal);
        static void scaleChunks(Physx::HeightFieldIndexedMesh& mesh, const LandscapeHeaderChunkExternal& extHeaderChunk, const LandscapeBodyChunkExternal& extBodyChunk);
        static void addTransforms(LandscapeInternal& landscape, const LandscapeHeaderExternal& externalHeader);

        // The Physx code to convert chunks into meshes is CPU-intensive.
        // Especially when large landscapes can reach 200 chunks (Soulframe MidrathRedux is 14x14).
        static std::vector<LandscapeChunkInternal> convertToInternalMultithread(const LandscapeHeaderExternal& landscapeHeader, const std::vector<LandscapeBodyChunkExternal>& landscapeBody);
        static void convertLandscapeTask(const LandscapeBodyChunkExternal* chunk, const LandscapeHeaderChunkExternal* eeChunkHeader, std::vector<LandscapeChunkInternal>* outputChunks, size_t outputChunkIndex);
    };
}