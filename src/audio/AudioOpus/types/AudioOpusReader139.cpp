#include "audio/AudioOpus/types/AudioOpusReader139.h"

using namespace WarframeExporter::Audio;

AudioOpusReader139*
AudioOpusReader139::getInstance()
{
	static AudioOpusReader139* instance = new AudioOpusReader139();
	return instance;
}

void
AudioOpusReader139::readBody(const AudioHeader& extHeader, BinaryReader::BinaryReaderBuffered* fReader, BinaryReader::BinaryReaderBuffered* bReader, AudioBody& outBody)
{
	std::vector<char>& data = outBody.data;

	if (fReader != nullptr)
	{
		data.resize(fReader->getLength());
		fReader->readUInt8Array((uint8_t*)data.data(), fReader->getLength());
	}
	if (bReader != nullptr && (fReader == nullptr || static_cast<int>(fReader->getLength()) != extHeader.size))
	{
		size_t curSize = data.size();
		data.resize(curSize + bReader->getLength());
		bReader->readUInt8Array((uint8_t*)data.data() + curSize, bReader->getLength());
	}	
}
