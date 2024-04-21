#pragma once

#include "LotusPath.h"

#include <vector>
#include <string>

namespace WarframeExporter::Material
{
    struct MaterialAttributes
    {
        std::string attributes;
        LotusLib::LotusPath filePath;
    };

    struct MaterialExternal
    {
        std::vector<std::string> attributes;
        std::vector<std::string> keywords;
        std::vector<MaterialAttributes> attributeChain;
        std::vector<std::string> hlm3Textures;
        std::vector<std::string> shaderSet1;
        std::vector<std::string> shaderSet2;
    };

    struct MaterialInternal
    {
        std::vector<std::pair<std::string, std::string>> shaderAttributes;
        std::vector<std::pair<std::string, std::string>> miscAttributes;
        std::vector<std::string> hlm3Textures;
        std::vector<std::string> shaderSet1;
        std::vector<std::string> shaderSet2;
    };
};