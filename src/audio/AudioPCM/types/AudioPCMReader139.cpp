#include "audio/AudioPCM/types/AudioPCMReader139.h"

using namespace WarframeExporter::Audio;

AudioPCMReader139*
AudioPCMReader139::getInstance()
{
	static AudioPCMReader139* instance = new AudioPCMReader139();
	return instance;
}

void
AudioPCMReader139::readBody(const AudioHeader& extHeader, BinaryReader::BinaryReaderBuffered* fReader, BinaryReader::BinaryReaderBuffered* bReader, AudioBody& outBody)
{
	std::vector<char>& data = outBody.data;

	size_t bLen = 0;
	if (bReader != nullptr)
	{
		bLen = bReader->getLength();
		data.resize(bReader->getLength());
		bReader->readUInt8Array((uint8_t*)data.data(), bReader->getLength());
	}
	if (fReader != nullptr)
	{
		data.resize(bLen + fReader->getLength());
		fReader->readUInt8Array((uint8_t*)data.data() + bLen, fReader->getLength());
	}	
}
