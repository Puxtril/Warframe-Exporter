#pragma once

#include "BinaryReaderBuffered.h"
#include "EnumMap.h"
#include "CommonHeader.h"
#include "levelstatic/LevelStaticStructs.h"
#include "ExporterExceptions.h"


namespace WarframeExporter::LevelStatic
{
    class LevelStaticReader : public EnumMapValue
    {
    public:
        virtual void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, LevelStaticHeaderExternal& outHeader) = 0;
        virtual void readBody(BinaryReader::BinaryReaderBuffered* bodyReader, const LevelStaticHeaderExternal& extHeader, LevelStaticBodyExternal& outBody) = 0;
    };
};