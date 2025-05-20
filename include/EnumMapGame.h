#pragma once

#include "LotusLib.h"

#include <map>
#include <stdexcept>
#include <vector>

namespace WarframeExporter
{
	// Same functionality as EnumMap, but with an added Game parameter
	class EnumMapGameValue
	{
	public:
		virtual std::vector<std::tuple<LotusLib::Game, int>> getEnumMapKeys() const = 0;
	};

	template <class T>
	class EnumMapGame
	{
		std::map<LotusLib::Game, std::map<int, T*>> m_enumMap;
	
	public:
		EnumMapGame()
		{
			static_assert(std::is_base_of<EnumMapGameValue, T>::value, "EnumMapGame value must be subclass of EnumMapGameValue");
		}
	
		EnumMapGame<T>&
		registerClass(T* cls)
		{
			std::vector<std::tuple<LotusLib::Game, int>> enums = cls->getEnumMapKeys();

			for (std::tuple<LotusLib::Game, int>& curEnum : enums)
			{
				LotusLib::Game  game = std::get<0>(curEnum);
				int type = std::get<1>(curEnum);

				if (m_enumMap[game].find(type) == m_enumMap[game].end())
					m_enumMap[game][type] = cls;
				else
					throw std::out_of_range("EnumMapGame key already exists");
			}
			return *this;
		}
	
		T*
		at(LotusLib::Game game, int type) const
		{
			try
			{
				return m_enumMap.at(game).at(type);
			}
			catch (std::out_of_range&)
			{
				return nullptr;
			}
		}
	};
}
