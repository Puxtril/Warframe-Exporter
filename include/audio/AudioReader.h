#pragma once

#include "EnumMapValue.h"
#include "BinaryReaderBuffered.h"
#include "Ensmallening.hpp"
#include "CommonHeader.h"
#include "AudioStructs.h"

namespace WarframeExporter::Audio
{	
	class AudioReader : public EnumMapValue
	{
	protected:
		AudioReader() = default;

	public:
		virtual void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) = 0;
		virtual void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AudioHeader& outHeader) = 0;
		virtual void readBody(const AudioHeader& extHeader, BinaryReaderBuffered* fReader, BinaryReaderBuffered* bReader, AudioBody& outBody) = 0;
	};
}
