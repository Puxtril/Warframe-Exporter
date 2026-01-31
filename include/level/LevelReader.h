#pragma once

#include "LevelStructs.h"
#include "BinaryReader/Buffered.h"
#include "ExporterExceptions.h"
#include "EnumMap.h"
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
		virtual void readHeader(BinaryReader::Buffered& reader, LevelHeaderExternal& outHeader) const = 0;
		virtual void readBody(BinaryReader::Buffered& reader, const LevelHeaderExternal& extHeader, LevelBodyExternal& outBody) const = 0;
	};
};
