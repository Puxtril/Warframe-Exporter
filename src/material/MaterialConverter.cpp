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
