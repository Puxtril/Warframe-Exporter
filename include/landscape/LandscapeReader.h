#pragma once

#include "BinaryReaderBuffered.h"
#include "EnumMap.h"
#include "CommonHeader.h"
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
        virtual LandscapeHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* reader) = 0;
        virtual std::vector<LandscapeBodyChunkExternal> readBody(BinaryReader::BinaryReaderBuffered* reader, const LandscapeHeaderExternal& header) = 0;

    protected:
        LandscapeBodyChunkExternal readPhysxHeightField(BinaryReader::BinaryReaderBuffered* reader);
    };
}