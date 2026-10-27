#pragma once

#include "EnumMap.h"
#include "audio/AudioReader.h"
#include "audio/AudioVorbis/types/AudioVorbisReader139.h"

namespace WarframeExporter::Audio
{
	const static EnumMap<AudioReader> g_enumMapAudioVorbisReader = EnumMap<AudioReader>()
		.registerClass(Audio::AudioVorbisReader139::getInstance());
}