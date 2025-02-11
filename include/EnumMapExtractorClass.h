#pragma once

#include "EnumMap.h"
#include "Extractor.h"

namespace WarframeExporter
{
	class EnumMapExtractor
	{
		std::map<LotusLib::Game, std::map<LotusLib::PackageCategory, std::map<int, Extractor*>>> m_enumMap;
	
	public:
		EnumMapExtractor&
		registerClass(Extractor* cls)
		{
			std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> enums = cls->getEnumMapKeys();
			for (std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>& x : enums)
			{
				LotusLib::Game game = std::get<0>(x);
				LotusLib::PackageCategory pkgCat = std::get<1>(x);
				int fileType = std::get<2>(x);

				// Insert maps if they don't already exist
				m_enumMap[game][pkgCat];

				if (m_enumMap[game][pkgCat].find(fileType) == m_enumMap[game][pkgCat].end())
					m_enumMap[game][pkgCat][fileType] = cls;
				else
					throw std::out_of_range("EnumMap key already exists");
			}
			return *this;
		}
	
		Extractor*
		at(LotusLib::Game game, LotusLib::PackageCategory pkgCat, int fileType) const
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
	};
}
