#pragma once

#include "EnumMap.h"
#include "audio/AudioReader.h"
#include "audio/AudioPCM/types/AudioPCMReader139.h"

namespace WarframeExporter::Audio
{
	const static EnumMap<AudioReader> g_enumMapAudioPCMReader = EnumMap<AudioReader>()
		.registerClass(Audio::AudioPCMReader139::getInstance());
}
