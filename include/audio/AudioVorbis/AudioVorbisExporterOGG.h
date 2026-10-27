#pragma once

#include "audio/AudioStructs.h"
#include "audio/AudioOpus/AudioOpusExporterOggChecksum.h"
#include "audio/AudioOpus/AudioOpusExporterOggStructs.h"
#include "audio/AudioVorbis/VorbisSetupHeaders.h"

#include "ExporterExceptions.h"

#include <ostream>
#include <vector>
#include <cstdint>

namespace WarframeExporter::Audio
{
	class AudioVorbisExporterOGG
	{
	public:
		static void writeData(const AudioHeader& header, const AudioBody& body, std::ostream& outFile);

	private:
		struct PacketSlice
		{
			const uint8_t* data;
			size_t len;
			uint64_t granulePosition;
		};

		static std::vector<uint8_t> createIdentificationHeader(const AudioHeader& header);
		static std::vector<uint8_t> createCommentHeader();
		static void writePage(const std::vector<PacketSlice>& packets, uint64_t granulePosition, uint32_t sequence, uint8_t flags, std::ostream& outFile);
	};
}