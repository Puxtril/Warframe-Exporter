#pragma once

#include "LotusLib.h"
#include <sstream>

#include "material/MaterialStructs.h"

namespace WarframeExporter::Material
{
    class MaterialConverter
    {
    public:
        static MaterialInternal convertMaterial(const MaterialExternal& externalMaterial, LotusLib::LotusPath& internalPath, LotusLib::PackagesReader& pkgs);
        static std::string combineMaterialToTxt(const MaterialInternal& internalMaterial);
        static std::string combineMaterialToJson(const MaterialInternal& internalMaterial);
        static void addPackagesBinHeirarchy(std::stringstream& outStr, LotusLib::PackageReader pkg, const std::string& filePath);

    private:
        static void combineAttributes(nlohmann::json& currentAttrs, const nlohmann::json& parentAttrs, const LotusLib::LotusPath& parentPath);
    };
};