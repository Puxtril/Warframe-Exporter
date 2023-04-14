#include "audio/AudioOpus/types/AudioOpusReader139.h"

using namespace WarframeExporter::Audio;

AudioOpusReader139*
AudioOpusReader139::getInstance()
{
	static AudioOpusReader139* instance = new AudioOpusReader139();
	return instance;
}

void
AudioOpusReader139::readBody(const AudioHeader& extHeader, BinaryReaderBuffered* fReader, BinaryReaderBuffered* bReader, AudioBody& outBody)
{
	std::vector<char>& data = outBody.data;

	if (fReader != nullptr)
	{
		data.resize(fReader->getLength());
		fReader->readUInt8Array((uint8_t*)data.data(), fReader->getLength());
	}
	if (bReader != nullptr && (fReader == nullptr || fReader->getLength() != extHeader.size))
	{
		data.resize(bReader->getLength() + fReader->getLength());
		bReader->readUInt8Array((uint8_t*)data.data() + fReader->getLength(), bReader->getLength());
	}	
}
