#pragma once

#include "LotusPath.h"

#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/gtx/quaternion.hpp"

#include <vector>
#include <string>

namespace WarframeExporter::LevelStatic
{
    struct LevelStaticHeaderExternal
    {
        uint32_t vertexCount;
        uint32_t indices1Count;
        uint32_t indices2Count;
        uint32_t matrixCount;
        uint32_t unk1Count;
        uint32_t unk2Count;
        std::vector<std::string> modelPaths;
        std::vector<std::string> naturalPaths;
        std::vector<std::string> attributes;
        std::vector<std::string> materialPaths;
    };

    struct LevelStaticObjectExternal
    {
        glm::vec3 pos;
        glm::quat rot;
        float scale;
        uint16_t modelIndex;
        uint16_t naturalIndex;
        uint16_t materialIndex;
        uint16_t matrixIndex;
    };

    struct LevelStaticBodyExternal
    {
        std::vector<LevelStaticObjectExternal> objects;
    };

    struct LevelStaticExternal
    {
        LevelStaticHeaderExternal header;
        LevelStaticBodyExternal body;
    };
};