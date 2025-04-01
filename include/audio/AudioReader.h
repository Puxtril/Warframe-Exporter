#pragma once

#include "EnumMap.h"
#include "BinaryReaderBuffered.h"
#include "CommonHeader.h"
#include "AudioStructs.h"

namespace WarframeExporter::Audio
{	
	class AudioReader : public EnumMapValue
	{
	protected:
		AudioReader() = default;

	public:
		virtual void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, AudioHeader& outHeader) = 0;
		virtual void readBody(const AudioHeader& extHeader, BinaryReader::BinaryReaderBuffered* fReader, BinaryReader::BinaryReaderBuffered* bReader, AudioBody& outBody) = 0;
	};
}
