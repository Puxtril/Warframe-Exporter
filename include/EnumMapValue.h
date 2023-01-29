#pragma once

#include <vector>

namespace WarframeExporter
{
	class EnumMapValue
	{
	public:
		virtual std::vector<int> getEnumMapKeys() const = 0;
	};
}
