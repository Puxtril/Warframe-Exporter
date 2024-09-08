#pragma once

#include "landscape/LandscapeReader.h"

namespace WarframeExporter::Landscape
{
    class LandscapeReader42 : public LandscapeReader
    {
        LandscapeReader42() = default;
        LandscapeReader42(const LandscapeReader42&) = default;

    public:
        static LandscapeReader42* getInstance();

        inline std::vector<int> getEnumMapKeys() const override
        {
            std::vector<int> extTypes = {
                (int)LandscapeType::LANDSCAPE_42,
            };
			return extTypes;
        }

        LandscapeHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* reader) override;
        std::vector<LandscapeBodyChunkExternal> readBody(BinaryReader::BinaryReaderBuffered* reader, const LandscapeHeaderExternal& header) override;
    };
}