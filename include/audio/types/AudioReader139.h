#pragma once

#include "audio/AudioReader.h"

namespace WarframeExporter::Audio
{
	class AudioReader139 : public AudioReader
	{
	protected:
		AudioReader139() = default;

	public:
		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)AudioType::AUDIO_139 };
			return extTypes;
		}

		void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) override;
		void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AudioHeader& outHeader) override;
	};
}
