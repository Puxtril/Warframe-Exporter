#include "material/MaterialConverter.h"

using namespace WarframeExporter::Material;

MaterialInternal
MaterialConverter::convertMaterial(const MaterialExternal& externalMaterial)
{
    MaterialInternal internal;

    internal.hlm3Textures = std::move(externalMaterial.hlm3Textures);
    internal.shaderSet1 = std::move(externalMaterial.shaderSet1);
    internal.shaderSet2 = std::move(externalMaterial.shaderSet2);

    splitAndCombineAttributes(externalMaterial.attributes, internal.shaderAttributes, internal.miscAttributes);
    for (const auto& x : externalMaterial.attributeChain)
        splitAndCombineAttributes(x.attributes, internal.shaderAttributes, internal.miscAttributes);

    return internal;
}

std::string
MaterialConverter::combineMaterial(const MaterialInternal& internalMaterial)
{
    std::stringstream formatted;

    formatted << "##################" << std::endl;
    formatted << "#   Attributes   #" << std::endl;
    formatted << "##################" << std::endl;
    formatted << std::endl;

    for (const auto& curAttribute : internalMaterial.miscAttributes)
        if (std::get<0>(curAttribute).size() > 1)
            formatted << std::get<0>(curAttribute) << " = " << std::get<1>(curAttribute) << std::endl;

    for (const auto& curAttribute : internalMaterial.shaderAttributes)
        if (std::get<0>(curAttribute).size() > 1)
            formatted << std::get<0>(curAttribute) << " = " << std::get<1>(curAttribute) << std::endl;

    if (internalMaterial.hlm3Textures.size() > 0)
    {
        formatted << std::endl;
        formatted << "####################" << std::endl;
        formatted << "#   HLM3 Textures  #" << std::endl;
        formatted << "####################" << std::endl;
        formatted << std::endl;

        for (const std::string& curTexture : internalMaterial.hlm3Textures)
        {
            formatted << curTexture << std::endl;
        }
    }

    formatted << std::endl;
    formatted << "####################" << std::endl;
    formatted << "#   Shader Set 1   #" << std::endl;
    formatted << "####################" << std::endl;
    formatted << std::endl;

    for (size_t i = 0; i < internalMaterial.shaderSet1.size(); i++)
    {
        formatted << i << ": " << internalMaterial.shaderSet1[i] << std::endl;
    }

    formatted << std::endl;
    formatted << "####################" << std::endl;
    formatted << "#   Shader Set 2   #" << std::endl;
    formatted << "####################" << std::endl;
    formatted << std::endl;

    for (size_t i = 0; i < internalMaterial.shaderSet2.size(); i++)
    {
        formatted << i << ": " << internalMaterial.shaderSet2[i] << std::endl;
    }

    return formatted.str();;
}

/*
std::string
MaterialConverter::combineMaterial(const MaterialInternal& internalMaterial)
{
    std::stringstream formatted;

    formatted << "################################" << std::endl;
    formatted << "#   Material File Attributes   #" << std::endl;
    formatted << "################################" << std::endl;
    formatted << std::endl;

    for (const std::string& curAttribute : externalMaterial.attributes)
    {
        formatted << curAttribute << std::endl;
    }

    for (size_t i = 0; i < externalMaterial.attributeChain.size(); i++)
    {
        const MaterialAttributes& curAttributes = externalMaterial.attributeChain[i];

        formatted << std::endl;
        formatted << "############################" << std::endl;
        formatted << "#   Heirarchy Attributes   #" << std::endl;
        formatted << "#        (Parent " << i << ")        #" << std::endl;
        formatted << "#############################" << std::endl;
        formatted << "Path: " << curAttributes.filePath << std::endl;
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
*/

void
MaterialConverter::splitAndCombineAttributes(const std::vector<std::string>& rawAttributes, std::map<std::string, std::string>& shaderAttributes, std::map<std::string, std::string>& miscAttributes)
{
    for (const std::string& curStr : rawAttributes)
    {
        std::string_view cur(curStr);
        size_t mid = cur.find('=');

        std::string key;
        std::string value;
        if (mid != std::string_view::npos)
        {
            key = std::string(cur.substr(0, mid));
            value = std::string(cur.substr(mid + 1, cur.length() - mid));
        }
        else
        {
            key = std::string(cur);
        }

        std::map<std::string, std::string>* insertIntoMap = &miscAttributes;
        if (key.find(':') != std::string::npos)
            insertIntoMap = &shaderAttributes;

        if (insertIntoMap->count(key) == 0)
            insertIntoMap->operator[](key) = value;
    }
}

void
MaterialConverter::splitAndCombineAttributes(const std::string& rawAttributes, std::map<std::string, std::string>& shaderAttributes, std::map<std::string, std::string>& miscAttributes)
{
    size_t last = 0;
    size_t next = 0;
    while ((next = rawAttributes.find('\n', last)) != std::string::npos)
    {
        std::string_view cur = std::string_view(rawAttributes).substr(last, next-last);
        size_t mid = cur.find('=');

        std::string key;
        std::string value;
        if (mid != std::string_view::npos)
        {
            key = std::string(cur.substr(0, mid));
            value = std::string(cur.substr(mid + 1, cur.length() - mid));
        }
        else
        {
            key = std::string(cur);
        }

        std::map<std::string, std::string>* insertIntoMap = &miscAttributes;
        if (key.find(':') != std::string::npos)
            insertIntoMap = &shaderAttributes;

        if (insertIntoMap->count(key) == 0)
            insertIntoMap->operator[](key) = value;

        last = next + 1;
    }
}