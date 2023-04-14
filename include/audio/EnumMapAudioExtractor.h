#pragma once

#include "EnumMap.h"
#include "audio/AudioOpus/AudioOpusExtractor.h"
#include "audio/AudioPCM/AudioPCMExtractor.h"

namespace WarframeExporter
{
	const static EnumMap<Extractor> g_enumMapAudioExtractor = EnumMap<Extractor>()
		.registerClass(Audio::AudioOpusExtractor::getInstance())
		.registerClass(Audio::AudioPCMExtractor::getInstance());
}
