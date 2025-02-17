#pragma once

#include "EnumMapValue.h"

#include <map>
#include <stdexcept>

namespace WarframeExporter
{
	// Map an enumeration (int) to a class
	// 
	// T must be a subclass of EnumMapValue.
	//  EnumMapValue only requires 1 function - to return the Enum (int) keys.
	// T should be a base class of your choosing.
	//  Ex. for model formats, T should have methods like readModel, processModel, etc.
	template <class T>
	class EnumMap
	{
		std::map<int, T*> m_enumMap;
	
	public:
		EnumMap()
		{
			static_assert(std::is_base_of<EnumMapValue, T>::value, "EnumMap value must be subclass of EnumMapValue");
		}
	
		EnumMap<T>&
		registerClass(T* cls)
		{
			std::vector<int> enums = cls->getEnumMapKeys();
			for (auto& x : enums)
			{
				if (m_enumMap.find(x) == m_enumMap.end())
					m_enumMap[x] = cls;
				else
					throw std::out_of_range("EnumMap key already exists");
			}
			return *this;
		}
	
		T*
		operator[](int x) const
		{
			try {
				return m_enumMap.at(x);
			} catch (std::out_of_range&) {
				return nullptr;
			}
		}
	};
}
