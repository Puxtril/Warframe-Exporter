#include "EnumMapExtractor.h"

using namespace WarframeExporter;

EnumMapExtractor&
EnumMapExtractor::registerClass(Extractor* cls)
{
    m_extractors.push_back(cls);
    std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> enums = cls->getEnumMapKeys();
    for (std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>& x : enums)
    {
        LotusLib::Game game = std::get<0>(x);
        LotusLib::PackageCategory pkgCat = std::get<1>(x);
        int fileType = std::get<2>(x);

        // Insert maps if they don't already exist
        m_enumMap[game][pkgCat];

        try
        {
            m_registeredPkgCategories[game].at(cls->getExtractorType());
        }
        catch (std::out_of_range&)
        {
            m_registeredPkgCategories[game][cls->getExtractorType()] = LotusLib::PackageCategory::UNKNOWN;
        }
        m_registeredPkgCategories[game][cls->getExtractorType()] = m_registeredPkgCategories[game][cls->getExtractorType()] | pkgCat;

        if (m_enumMap[game][pkgCat].find(fileType) == m_enumMap[game][pkgCat].end())
            m_enumMap[game][pkgCat][fileType] = cls;
        else
            throw std::out_of_range("EnumMap key already exists");
    }
    return *this;
}

Extractor*
EnumMapExtractor::at(LotusLib::Game game, LotusLib::PackageCategory pkgCat, int fileType) const
{
    try
    {
        return m_enumMap.at(game).at(pkgCat).at(fileType);
    }
    catch (std::out_of_range&)
    {
        return nullptr;
    }
}

LotusLib::PackageCategory
EnumMapExtractor::getPkgCategories(LotusLib::Game game, ExtractorType extractorType) const
{
    LotusLib::PackageCategory categories = LotusLib::PackageCategory::UNKNOWN;
    for (Extractor* curExtractor : m_extractors)
    {
        if (((int)curExtractor->getExtractorType() & (int)extractorType) == 0)
            continue;

        try
        {
            categories = categories | m_registeredPkgCategories.at(game).at(curExtractor->getExtractorType());
        }
        catch (std::out_of_range&)
        {
            continue;
        }
    }
    return categories;
}

LotusLib::PackageCategory
EnumMapExtractor::getPkgCategories(LotusLib::Game game) const
{
    LotusLib::PackageCategory categories = LotusLib::PackageCategory::UNKNOWN;
    for (Extractor* extractor : m_extractors)
    {
        try
        {
            categories = categories | m_registeredPkgCategories.at(game).at(extractor->getExtractorType());
        }
        catch (std::out_of_range&)
        {
            continue;
        }
    }
    return categories;
}

const std::vector<Extractor*>
EnumMapExtractor::getRegisteredExtractors() const
{
    return m_extractors;
}