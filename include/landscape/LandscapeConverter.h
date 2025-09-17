#pragma once

#include "landscape/LandscapeStructs.h"
#include "glm/gtx/quaternion.hpp"
#include "ExporterLogger.h"
#include "ExporterExceptions.h"

#include <cmath>
#include <tuple>

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
        static void scaleChunks(Physx::HeightFieldMesh& mesh, const LandscapeHeaderChunkExternal& extHeaderChunk, const LandscapeBodyChunkExternal& extBodyChunk);
        static void addTransforms(LandscapeInternal& landscape);
    };
}