#pragma once

#include "material/MaterialStructs.h"

namespace WarframeExporter::Material
{
    class MaterialExporter
    {
    public:
        static std::string combineMaterialToTxt(const MaterialInternal& internalMaterial);
        static std::string combineMaterialToJson(const MaterialInternal& internalMaterial);
    };
}