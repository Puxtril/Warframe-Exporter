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
	class LevelReader20x : public LevelReader
	{
	protected:
		LevelReader20x() = default;
		
	public:
		inline static LevelReader20x* getInstance()
		{
			static LevelReader20x* instance = new LevelReader20x();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
				(int)LevelType::LEVEL_201,
				(int)LevelType::LEVEL_202,
				(int)LevelType::LEVEL_207
			};
			return extTypes;
		}
	
		void readHeader(BinaryReader::Buffered& reader, LevelHeaderExternal& outHeader) const override;
		void readBody(BinaryReader::Buffered& reader, const LevelHeaderExternal& extHeader, LevelBodyExternal& outBody) const override;

	};
};
