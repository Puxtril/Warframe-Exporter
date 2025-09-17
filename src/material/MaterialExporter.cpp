#include "material/MaterialExporter.h"

using namespace WarframeExporter::Material;

std::string
MaterialExporter::combineMaterialToTxt(const MaterialInternal& internalMaterial)
{
    std::stringstream outStream;

    outStream << "##################" << std::endl;
    outStream << "#   Attributes   #" << std::endl;
    outStream << "##################" << std::endl;
    outStream << std::endl;

    for (const auto& curAttribute : internalMaterial.shaderAttributes.items())
    {
        if (curAttribute.key()[2] != ':')
        {
            if (curAttribute.value().is_string())
                outStream << curAttribute.key() << " = " << curAttribute.value().get<std::string>() << std::endl;
            else
                outStream << curAttribute.key() << " = " << curAttribute.value() << std::endl;
        }
    }

    for (const auto& curAttribute : internalMaterial.shaderAttributes.items())
    {
        if (curAttribute.key()[2] == ':')
        {
            if (curAttribute.value().is_string())
                outStream << curAttribute.key() << " = " << curAttribute.value().get<std::string>() << std::endl;
            else
                outStream << curAttribute.key() << " = " << curAttribute.value() << std::endl;
        }
    }

    if (internalMaterial.hlm3Textures.size() > 0)
    {
        outStream << std::endl;
        outStream << "####################" << std::endl;
        outStream << "#   HLM3 Textures  #" << std::endl;
        outStream << "####################" << std::endl;
        outStream << std::endl;

        for (const std::string& curTexture : internalMaterial.hlm3Textures)
        {
            outStream << curTexture << std::endl;
        }
    }

    outStream << std::endl;
    outStream << "####################" << std::endl;
    outStream << "#   Shader Set 1   #" << std::endl;
    outStream << "####################" << std::endl;
    outStream << std::endl;

    for (size_t i = 0; i < internalMaterial.shaderSet1.size(); i++)
    {
        outStream << i << ": " << internalMaterial.shaderSet1[i] << std::endl;
    }

    outStream << std::endl;
    outStream << "####################" << std::endl;
    outStream << "#   Shader Set 2   #" << std::endl;
    outStream << "####################" << std::endl;
    outStream << std::endl;

    for (size_t i = 0; i < internalMaterial.shaderSet2.size(); i++)
    {
        outStream << i << ": " << internalMaterial.shaderSet2[i] << std::endl;
    }

    return outStream.str();
}

std::string
MaterialExporter::combineMaterialToJson(const MaterialInternal& internalMaterial)
{
    nlohmann::json outputJson = nlohmann::json::object();
    outputJson["attributes"] = internalMaterial.shaderAttributes;

    outputJson["hlm3"] = nlohmann::json::array();
    for (const std::string& curTexture : internalMaterial.hlm3Textures)
        outputJson["hlm3"].push_back(curTexture);

    outputJson["shaders1"] = nlohmann::json::array();
    for (const std::string& curShader : internalMaterial.shaderSet1)
        outputJson["shaders1"].push_back(curShader);

    outputJson["shaders2"] = nlohmann::json::array();
    for (const std::string& curShader : internalMaterial.shaderSet2)
        outputJson["shaders2"].push_back(curShader);

    return outputJson.dump(4);
}