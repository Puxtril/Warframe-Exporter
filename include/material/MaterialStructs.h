#pragma once

#include "LotusPath.h"
#include "LotusNotationParser.h"

#include <vector>
#include <string>

namespace WarframeExporter::Material
{
    struct MaterialExternal
    {
        std::string attributes;
        std::vector<std::string> keywords;
        std::vector<std::string> hlm3Textures;
        std::vector<std::string> shaderSet1;
        std::vector<std::string> shaderSet2;
    };

    struct MaterialInternal
    {
        nlohmann::json shaderAttributes;
        std::vector<std::string> hlm3Textures;
        std::vector<std::string> shaderSet1;
        std::vector<std::string> shaderSet2;
    };
};