#pragma once

#include "landscape/LandscapeStructs.h"
#include "glm/gtx/quaternion.hpp"

#include <set>
#include <iostream>

namespace WarframeExporter::Landscape
{
    class LandscapeConverter
    {
    public:
        static void positionChunks(const LandscapeHeaderExternal& externalHeader, const std::vector<LandscapeBodyChunkExternal>& externalChunks, LandscapeInternal& internal);
        static void scaleChunks(Physx::HeightFieldMeshSplit& mesh, const LandscapeHeaderChunkExternal& extHeaderChunk, const LandscapeBodyChunkExternal& extBodyChunk);
    };
}