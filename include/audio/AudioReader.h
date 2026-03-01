#pragma once

#include "EnumMap.h"
#include "BinaryReader/Buffered.h"
#include "LotusLib/CommonHeader.h"
#include "AudioStructs.h"

namespace WarframeExporter::Audio
{	
	class AudioReader : public EnumMapValue
	{
	protected:
		AudioReader() = default;

	public:
		virtual void readHeader(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& header, AudioHeader& outHeader) = 0;
		virtual void readBody(const AudioHeader& extHeader, BinaryReader::Buffered* fReader, BinaryReader::Buffered* bReader, AudioBody& outBody) = 0;
	};
}
