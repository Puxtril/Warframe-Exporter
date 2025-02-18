#pragma once

#include "landscape/LandscapeReader.h"

namespace WarframeExporter::Landscape
{
    class LandscapeReader4X : public LandscapeReader
    {
        LandscapeReader4X() = default;
        LandscapeReader4X(const LandscapeReader4X&) = default;

    public:
        static LandscapeReader4X* getInstance();

        inline std::vector<int> getEnumMapKeys() const override
        {
            std::vector<int> extTypes = {
                (int)LandscapeType::LANDSCAPE_42,
                (int)LandscapeType::LANDSCAPE_43
            };
			return extTypes;
        }

        LandscapeHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* reader) override;
        std::vector<LandscapeBodyChunkExternal> readBody(BinaryReader::BinaryReaderBuffered* reader, const LandscapeHeaderExternal& header) override;
    };
}