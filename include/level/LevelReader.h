#pragma once

#include "LevelStructs.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"
#include "EnumMapValue.h"
#include "level/LevelTypes.h"

#include <string>
#include <cstdint>

namespace WarframeExporter::Level
{
	class LevelReader : public EnumMapValue
	{
	protected:
		LevelReader() = default;
		
	public:	
		virtual void readHeader(BinaryReader::BinaryReaderBuffered& reader, LevelHeaderExternal& outHeader) const = 0;
		virtual void readBody(BinaryReader::BinaryReaderBuffered& reader, const LevelHeaderExternal& extHeader, LevelBodyExternal& outBody) const = 0;
	};
};
