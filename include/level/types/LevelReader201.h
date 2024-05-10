#pragma once

#include "level/LevelStructs.h"
#include "level/LevelReader.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"

#include <string>
#include <cstdint>
#include <iostream>
#include <regex>

namespace WarframeExporter::Level
{
	class LevelReader201 : public LevelReader
	{
	protected:
		LevelReader201() = default;
		
	public:
		inline static LevelReader201* getInstance()
		{
			static LevelReader201* instance = new LevelReader201();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)LevelType::LEVEL_201 };
			return extTypes;
		}
	
		void readHeader(BinaryReader::BinaryReaderBuffered& reader, LevelHeaderExternal& outHeader) const override;
		void readBody(BinaryReader::BinaryReaderBuffered& reader, const LevelHeaderExternal& extHeader, LevelBodyExternal& outBody) const override;

	};
};
