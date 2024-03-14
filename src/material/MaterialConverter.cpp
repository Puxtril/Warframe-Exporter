#include "material/MaterialConverter.h"

using namespace WarframeExporter::Material;

MaterialInternal
MaterialConverter::convertMaterial(const MaterialExternal& externalMaterial)
{
    std::stringstream formatted;

    formatted << "------------------" << std::endl;
    formatted << "|   Attributes   |" << std::endl;
    formatted << "------------------" << std::endl;
    formatted << std::endl;

    for (const std::string& curAttribute : externalMaterial.attributes)
    {
        formatted << curAttribute << std::endl;
    }

    formatted << "------------------" << std::endl;
    formatted << "|   Parameters   |" << std::endl;
    formatted << "------------------" << std::endl;
    formatted << std::endl;

    for (const std::string& curKeyword : externalMaterial.keywords)
    {
        formatted << curKeyword << std::endl;
    }

    if (externalMaterial.hlm3Textures.size() > 0)
    {
        formatted << std::endl;
        formatted << "--------------------" << std::endl;
        formatted << "|   HLM3 Textures  |" << std::endl;
        formatted << "--------------------" << std::endl;
        formatted << std::endl;

        for (const std::string& curTexture : externalMaterial.hlm3Textures)
        {
            formatted << curTexture << std::endl;
        }
    }

    MaterialInternal internalMaterial;
    internalMaterial.formatted = formatted.str();
    return internalMaterial;
}
