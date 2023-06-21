#pragma once

#include "EnumMap.h"
#include "audio/AudioReader.h"
#include "audio/AudioOpus/types/AudioOpusReader139.h"

namespace WarframeExporter::Audio
{
	const static EnumMap<AudioReader> g_enumMapAudioOpusReader = EnumMap<AudioReader>()
		.registerClass(Audio::AudioOpusReader139::getInstance());
}
