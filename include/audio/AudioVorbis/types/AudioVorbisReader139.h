#pragma once

#include "audio/types/AudioReader139.h"

#include "ExporterExceptions.h"

namespace WarframeExporter::Audio
{
    class AudioVorbisReader139 : public AudioReader139
    {
        AudioVorbisReader139() = default;

    public:
        static AudioVorbisReader139* getInstance();
        void readBody(const AudioHeader& extHeader, BinaryReader::Buffered* fReader, BinaryReader::Buffered* bReader, AudioBody& outBody) override;
    };
}