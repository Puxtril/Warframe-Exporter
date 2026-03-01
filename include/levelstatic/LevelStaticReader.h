#pragma once

#include "BinaryReader/Buffered.h"
#include "EnumMap.h"
#include "LotusLib/CommonHeader.h"
#include "levelstatic/LevelStaticStructs.h"
#include "ExporterExceptions.h"


namespace WarframeExporter::LevelStatic
{
    class LevelStaticReader : public EnumMapValue
    {
    public:
        virtual void readHeader(BinaryReader::Buffered* headerReader, LevelStaticHeaderExternal& outHeader) = 0;
        virtual void readBody(BinaryReader::Buffered* bodyReader, const LevelStaticHeaderExternal& extHeader, LevelStaticBodyExternal& outBody) = 0;
    };
};