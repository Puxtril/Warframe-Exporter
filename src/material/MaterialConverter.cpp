#include "material/MaterialConverter.h"

using namespace WarframeExporter::Material;

MaterialInternal
MaterialConverter::convertMaterial(const MaterialExternal& externalMaterial, LotusLib::LotusPath& internalPath, LotusLib::PackagesReader& pkgs)
{
    MaterialInternal internal;

    internal.hlm3Textures = std::move(externalMaterial.hlm3Textures);
    internal.shaderSet1 = std::move(externalMaterial.shaderSet1);
    internal.shaderSet2 = std::move(externalMaterial.shaderSet2);

    // Essentially, this is setting `internal.shaderAttributes` to `currentJson`.
    // But, we still want to fix relative file paths.
    nlohmann::json currentJson = LotusLib::LotusNotationParser::parse(externalMaterial.attributes.c_str(), externalMaterial.attributes.length());
    internal.shaderAttributes = nlohmann::json::object();
    combineAttributes(internal.shaderAttributes, currentJson, internalPath);
    
    // Go through Packages.bin to add material heirarchy
    pkgs.initilizePackagesBin();
    LotusLib::PackageReader pkg = pkgs.getPackage("Misc").value();
    LotusLib::LotusPath currentPath = internalPath;
    while (currentPath != "")
    {
        LotusLib::FileEntry entry = pkg.getFile(currentPath, LotusLib::READ_EXTRA_ATTRIBUTES);
        nlohmann::json parentAttrs = LotusLib::LotusNotationParser::parse(entry.extra.attributes.c_str(), entry.extra.attributes.size());
        combineAttributes(internal.shaderAttributes, parentAttrs, currentPath);
        currentPath = entry.extra.parent;
    }

    return internal;
}

std::string
MaterialConverter::combineMaterialToTxt(const MaterialInternal& internalMaterial)
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
MaterialConverter::combineMaterialToJson(const MaterialInternal& internalMaterial)
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

void
MaterialConverter::addPackagesBinHeirarchy(std::stringstream& outStr, LotusLib::PackageReader pkg, const std::string& filePath)
{
    std::stack<std::string> heirarchy;

    std::string curFile = filePath;
    while (curFile != "")
    {
        heirarchy.push(curFile);
        LotusLib::FileEntry nextEntry = pkg.getFile(curFile, LotusLib::READ_EXTRA_ATTRIBUTES);
        curFile = nextEntry.extra.parent.string();
    }

    if (heirarchy.size() == 1)
        return;

    outStr << std::endl << std::endl;

    outStr << "Hierarchy" << std::endl;
    outStr << "----------" << std::endl;

    int curLevel = 1;
    while (!heirarchy.empty())
    {
        outStr << "[" << curLevel++ << "]  " << heirarchy.top() << std::endl;
        heirarchy.pop();
    }
}

void
MaterialConverter::combineAttributes(nlohmann::json& currentAttrs, const nlohmann::json& parentAttrs, const LotusLib::LotusPath& parentPath)
{
    for (const auto& curJson : parentAttrs.items())
    {
        if (!currentAttrs.contains(curJson.key()))
        {
            if (curJson.key()[0] == 'T' && curJson.key()[1] == 'X' && curJson.key()[2] == ':')
            {
                // Must cast `curJson.value()` to `std::string` to use `operator[]`
                std::string value = curJson.value().get<std::string>();
                if (value[0] != '/')
                {
                    currentAttrs[curJson.key()] = parentPath.string() + "/" + value;
                    continue;
                }
            }
            currentAttrs[curJson.key()] = curJson.value();
        }
    }
}
