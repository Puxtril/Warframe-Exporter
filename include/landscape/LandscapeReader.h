#pragma once

#include "BinaryReader/Buffered.h"
#include "EnumMap.h"
#include "LotusLib/CommonHeader.h"
#include "ExporterLogger.h"
#include "ExporterExceptions.h"
#include "landscape/LandscapeStructs.h"
#include "landscape/LandscapeTypes.h"
#include "heightfield/Reader.h"

namespace WarframeExporter::Landscape
{
    class LandscapeReader : public EnumMapValue
    {
    public:
        virtual LandscapeHeaderExternal readHeader(BinaryReader::Buffered* reader) = 0;
        virtual std::vector<LandscapeBodyChunkExternal> readBody(BinaryReader::Buffered* reader, const LandscapeHeaderExternal& header) = 0;

    protected:
        LandscapeBodyChunkExternal readPhysxHeightField(BinaryReader::Buffered* reader);
    };
}