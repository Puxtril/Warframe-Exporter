#pragma once

#include "level/LevelStructs.h"
#include "level/LevelReader.h"
#include "BinaryReader/Buffered.h"
#include "ExporterExceptions.h"

#include <string>
#include <cstdint>
#include <iostream>
#include <regex>

namespace WarframeExporter::Level
{
	class LevelReader212 : public LevelReader
	{
	protected:
		LevelReader212() = default;
		
	public:
		inline static LevelReader212* getInstance()
		{
			static LevelReader212* instance = new LevelReader212();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
				(int)LevelType::LEVEL_212,
			};
			return extTypes;
		}
	
		void readHeader(BinaryReader::Buffered& reader, LevelHeaderExternal& outHeader) const override;
		void readBody(BinaryReader::Buffered& reader, const LevelHeaderExternal& extHeader, LevelBodyExternal& outBody) const override;

	};
};
