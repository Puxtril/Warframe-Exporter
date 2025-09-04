#pragma once

#include "LotusLib.h"

#include "material/MaterialStructs.h"

namespace WarframeExporter::Material
{
    class MaterialConverter
    {
    public:
        static MaterialInternal convertMaterial(const MaterialExternal& externalMaterial, LotusLib::LotusPath& internalPath, LotusLib::PackagesReader& pkgs);

    private:
        static void combineAttributes(nlohmann::json& currentAttrs, const nlohmann::json& parentAttrs, const LotusLib::LotusPath& parentPath);
    };
};