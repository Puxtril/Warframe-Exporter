#pragma once

#include "LotusLib/PackageCollection.h"

#include "LotusLib/PackagesBin.h"
#include "material/MaterialStructs.h"

namespace WarframeExporter::Material
{
    class MaterialConverter
    {
    public:
        static MaterialInternal convertMaterial(const MaterialExternal& externalMaterial, const std::string& internalPath, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin);

    private:
        static void combineAttributes(nlohmann::json& currentAttrs, const nlohmann::json& parentAttrs, const std::string& parentPath);
    };
};