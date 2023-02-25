#pragma once

#include "level/LevelStructs.h"
#include "level/LevelReader.h"
#include "BinaryReaderBase.h"
#include "ExporterExceptions.h"

#include <string>
#include <cstdint>

namespace WarframeExporter::Level
{
	class LevelReader196 : public LevelReader
	{
	protected:
		LevelReader196() = default;
		
	public:
		inline static LevelReader196* getInstance()
		{
			static LevelReader196* instance = new LevelReader196();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)LevelType::LEVEL_196 };
			return extTypes;
		}
	
		void readHeaderDebug(BinaryReaderBase& reader, LevelHeaderExternal& outHeader) const override;
		void readBodyDebug(BinaryReaderBase& reader, const LevelHeaderExternal& extHeader) const override;
		void readHeader(BinaryReaderBase& reader, LevelHeaderExternal& outHeader) const override;
	};
};
