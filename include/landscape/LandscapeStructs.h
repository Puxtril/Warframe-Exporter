#pragma once

#include "glm/ext/vector_float3.hpp"
#include "glm/vec3.hpp"
#include "glm/matrix.hpp"
#include "heightfield/Structs.h"

#include <vector>
#include <string>

namespace WarframeExporter::Landscape
{
    struct LandscapeHeaderChunkExternal
    {
        glm::vec3 scale;
        uint32_t bodyLen;
    };

    struct LandscapeHeaderExternal
    {
        uint32_t rowCount;
        uint32_t columnCount;
        std::vector<std::vector<std::string>> materialPathArrays;
        std::vector<LandscapeHeaderChunkExternal> chunks;
    };

    struct LandscapeBodyChunkExternal
    {
        Physx::HeightFieldHeader header;
        std::vector<Physx::HeightFieldSample> samples;
    };

    struct LandscapeInternal
    {
        std::vector<glm::vec3> positions;
        std::vector<Physx::HeightFieldMesh> chunks;
        std::vector<std::vector<std::string>> materialPathArrays;
        std::vector<glm::mat4> transforms;
    };
}