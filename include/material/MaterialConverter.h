#pragma once

#include <sstream>
#include <algorithm>

#include "material/MaterialStructs.h"

namespace WarframeExporter::Material
{
    class MaterialConverter
    {
    public:
        static MaterialInternal convertMaterial(const MaterialExternal& externalMaterial);
        static std::string combineMaterial(const MaterialInternal& internalMaterial);
        static void replaceCurlyBracketsWithSquare(MaterialInternal& internalMaterial);

    private:
        static void splitAndCombineAttributes(
            const std::vector<std::string>& rawAttributes,
            std::map<std::string, std::string>& shaderAttributes,
            std::map<std::string, std::string>& miscAttributes
        );

        static void splitAndCombineAttributes(
            const std::string& rawAttributes,
            std::map<std::string, std::string>& shaderAttributes,
            std::map<std::string, std::string>& miscAttributes
        );
    };
};