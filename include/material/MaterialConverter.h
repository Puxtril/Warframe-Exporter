#pragma once

#include "LotusLib.h"
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
        static void combineMaterial(std::stringstream& outStream, const MaterialInternal& internalMaterial);
        static void replaceCurlyBracketsWithSquare(MaterialInternal& internalMaterial);
        static void addPackagesBinHeirarchy(std::stringstream& outStr, LotusLib::PackageReader pkg, const std::string& filePath);

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