#include "material/MaterialConverter.h"

using namespace WarframeExporter::Material;

MaterialInternal
MaterialConverter::convertMaterial(const MaterialExternal& externalMaterial)
{
    MaterialInternal internal;

    internal.hlm3Textures = std::move(externalMaterial.hlm3Textures);
    internal.shaderSet1 = std::move(externalMaterial.shaderSet1);
    internal.shaderSet2 = std::move(externalMaterial.shaderSet2);

    std::set<std::string> seenAttributes;

    splitAndCombineAttributes(externalMaterial.attributes, internal.shaderAttributes, internal.miscAttributes, seenAttributes);
    for (const auto& x : externalMaterial.attributeChain)
        splitAndCombineAttributes(x.attributes, internal.shaderAttributes, internal.miscAttributes, seenAttributes);

    return internal;
}

void
MaterialConverter::replaceCurlyBracketsWithSquare(MaterialInternal& internalMaterial)
{
    for (auto& curPair : internalMaterial.shaderAttributes)
    {
        std::replace(std::get<1>(curPair).begin(), std::get<1>(curPair).end(), '{', '[');
        std::replace(std::get<1>(curPair).begin(), std::get<1>(curPair).end(), '}', ']');
    }
    for (auto& curPair : internalMaterial.miscAttributes)
    {
        std::replace(std::get<1>(curPair).begin(), std::get<1>(curPair).end(), '{', '[');
        std::replace(std::get<1>(curPair).begin(), std::get<1>(curPair).end(), '}', ']');
    }
}

void
MaterialConverter::combineMaterial(std::stringstream& outStream, const MaterialInternal& internalMaterial)
{
    outStream << "##################" << std::endl;
    outStream << "#   Attributes   #" << std::endl;
    outStream << "##################" << std::endl;
    outStream << std::endl;

    for (const auto& curAttribute : internalMaterial.miscAttributes)
        if (std::get<0>(curAttribute).size() > 1)
            outStream << std::get<0>(curAttribute) << " = " << std::get<1>(curAttribute) << std::endl;

    for (const auto& curAttribute : internalMaterial.shaderAttributes)
        if (std::get<0>(curAttribute).size() > 1)
            outStream << std::get<0>(curAttribute) << " = " << std::get<1>(curAttribute) << std::endl;

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
MaterialConverter::splitAndCombineAttributes(
    const std::vector<std::string>& rawAttributes,
    std::vector<std::pair<std::string, std::string>>& shaderAttributes,
    std::vector<std::pair<std::string, std::string>>& miscAttributes,
    std::set<std::string>& seenAttributes
)
{
    for (const std::string& curStr : rawAttributes)
    {
        std::string_view cur(curStr);
        splitAndCombineAttribute(cur, shaderAttributes, miscAttributes, seenAttributes);
    }
}

void
MaterialConverter::splitAndCombineAttributes(
    const std::string& rawAttributes,
    std::vector<std::pair<std::string, std::string>>& shaderAttributes,
    std::vector<std::pair<std::string, std::string>>& miscAttributes,
    std::set<std::string>& seenAttributes
)
{
    size_t last = 0;
    size_t next = 0;
    while ((next = rawAttributes.find('\n', last)) != std::string::npos)
    {
        std::string_view cur = std::string_view(rawAttributes).substr(last, next-last);
        splitAndCombineAttribute(cur, shaderAttributes, miscAttributes, seenAttributes);
        last = next + 1;
    }
}

void
MaterialConverter::splitAndCombineAttribute(
    std::string_view& curAttribute,
    std::vector<std::pair<std::string, std::string>>& shaderAttributes,
    std::vector<std::pair<std::string, std::string>>& miscAttributes,
    std::set<std::string>& seenAttributes
)
{
    size_t mid = curAttribute.find('=');

    std::string key;
    std::string value;
    if (mid != std::string_view::npos)
    {
        key = std::string(curAttribute.substr(0, mid));
        value = std::string(curAttribute.substr(mid + 1, curAttribute.length() - mid));
    }
    else
    {
        key = std::string(curAttribute);
    }

    std::vector<std::pair<std::string, std::string>>* insertInto = &miscAttributes;
    if (key.find(':') != std::string::npos)
        insertInto = &shaderAttributes;

    if (seenAttributes.count(key) == 0)
    {
        insertInto->push_back({key, value});
        seenAttributes.insert(key);
    }
}