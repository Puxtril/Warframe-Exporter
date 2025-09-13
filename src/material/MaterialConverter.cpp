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
                    currentAttrs[curJson.key()] = parentPath.parent_path().string() + "/" + value;
                    continue;
                }
            }
            currentAttrs[curJson.key()] = curJson.value();
        }
    }
}
