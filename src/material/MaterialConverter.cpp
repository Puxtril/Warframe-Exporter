#include "material/MaterialConverter.h"

using namespace WarframeExporter::Material;

MaterialInternal
MaterialConverter::convertMaterial(const MaterialExternal& externalMaterial)
{
    std::stringstream formatted;

    formatted << "##################" << std::endl;
    formatted << "#   Attributes   #" << std::endl;
    formatted << "##################" << std::endl;
    formatted << std::endl;

    for (const std::string& curAttribute : externalMaterial.attributes)
    {
        formatted << curAttribute << std::endl;
    }

    formatted << "##################" << std::endl;
    formatted << "#   Parameters   #" << std::endl;
    formatted << "##################" << std::endl;
    formatted << std::endl;

    for (const std::string& curKeyword : externalMaterial.keywords)
    {
        formatted << curKeyword << std::endl;
    }

    for (size_t i = 0; i < externalMaterial.attributeChain.size(); i++)
    {
        const MaterialAttributes& curAttributes = externalMaterial.attributeChain[i];

        formatted << std::endl;
        formatted << "#############################" << std::endl;
        formatted << "#   Attributes (Parent " << i << ")   #" << std::endl;
        formatted << "#############################" << std::endl;
        formatted << curAttributes.filePath << std::endl;
        formatted << std::endl;
        formatted << curAttributes.attributes << std::endl;
    }

    if (externalMaterial.hlm3Textures.size() > 0)
    {
        formatted << std::endl;
        formatted << "####################" << std::endl;
        formatted << "#   HLM3 Textures  #" << std::endl;
        formatted << "####################" << std::endl;
        formatted << std::endl;

        for (const std::string& curTexture : externalMaterial.hlm3Textures)
        {
            formatted << curTexture << std::endl;
        }
    }

    formatted << std::endl;
    formatted << "####################" << std::endl;
    formatted << "#   Shader Set 1   #" << std::endl;
    formatted << "####################" << std::endl;
    formatted << std::endl;

    for (size_t i = 0; i < externalMaterial.shaderSet1.size(); i++)
    {
        formatted << i << ": " << externalMaterial.shaderSet1[i] << std::endl;
    }

    formatted << std::endl;
    formatted << "####################" << std::endl;
    formatted << "#   Shader Set 2   #" << std::endl;
    formatted << "####################" << std::endl;
    formatted << std::endl;

    for (size_t i = 0; i < externalMaterial.shaderSet2.size(); i++)
    {
        formatted << i << ": " << externalMaterial.shaderSet2[i] << std::endl;
    }

    MaterialInternal internalMaterial;
    internalMaterial.formatted = formatted.str();
    return internalMaterial;
}
