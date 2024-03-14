#pragma once

#include <vector>
#include <string>

namespace WarframeExporter::Material
{
    struct MaterialExternal
    {
        std::vector<std::string> attributes;
        std::vector<std::string> keywords;
        std::vector<std::string> hlm3Textures;
    };

    struct MaterialInternal
    {
        std::string formatted;
    };
};