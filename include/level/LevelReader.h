#pragma once

#include "LevelStructs.h"
#include "BinaryReaderBase.h"
#include "ExporterExceptions.h"
#include "EnumMapValue.h"

#include <string>
#include <cstdint>

namespace WarframeExporter::Level
{
	enum class LevelType
	{
		LEVEL_201 = 201
	};
	
	class LevelReader : public EnumMapValue
	{
	protected:
		LevelReader() = default;
		
	public:	
		virtual void readHeaderDebug(BinaryReaderBase& reader) const = 0;
		virtual void readHeader(BinaryReaderBase& reader, LevelHeaderExternal& outHeader) const = 0;
		virtual void readBodyDebug(BinaryReaderBase& reader, const LevelHeaderExternal& extHeader) const = 0;
		virtual void readBody(BinaryReaderBase& reader, const LevelHeaderExternal& extHeader, LevelBodyExternal& outBody) const = 0;
	};
};
