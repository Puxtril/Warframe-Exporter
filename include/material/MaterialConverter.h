#pragma once

#include <sstream>
#include <algorithm>
#include <set>

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
            std::vector<std::pair<std::string, std::string>>& shaderAttributes,
            std::vector<std::pair<std::string, std::string>>& miscAttributes,
            std::set<std::string>& seenAttributes
        );

        static void splitAndCombineAttributes(
            const std::string& rawAttributes,
            std::vector<std::pair<std::string, std::string>>& shaderAttributes,
            std::vector<std::pair<std::string, std::string>>& miscAttributes,
            std::set<std::string>& seenAttributes
        );

        static void splitAndCombineAttribute(
            std::string_view& curAttribute,
            std::vector<std::pair<std::string, std::string>>& shaderAttributes,
            std::vector<std::pair<std::string, std::string>>& miscAttributes,
            std::set<std::string>& seenAttributes
        );
    };
};