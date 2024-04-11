#pragma once

#include <vector>
#include <string>
#include <map>

namespace WarframeExporter::Material
{
    struct MaterialAttributes
    {
        std::string attributes;
        std::string filePath;
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
        std::map<std::string, std::string> shaderAttributes;
        std::map<std::string, std::string> miscAttributes;
        std::vector<std::string> hlm3Textures;
        std::vector<std::string> shaderSet1;
        std::vector<std::string> shaderSet2;
    };
};