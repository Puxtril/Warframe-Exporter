#pragma once

#include "EnumMapExtractorClass.h"
#include "audio/AudioOpus/AudioOpusExtractor.h"
#include "audio/AudioPCM/AudioPCMExtractor.h"

namespace WarframeExporter
{
	/* So... Unfortunately an edge case and perhaps strange design decision.
	 * File formats are separated by games and packages, but Opus and PCM are game-agnostic.
	 * So to benefit from the extractor enum map... Game should always be WARFRAME and package should always be MISC
	 */
	const static EnumMapExtractor g_enumMapAudioExtractor = EnumMapExtractor()
		.registerClass(Audio::AudioOpusExtractor::getInstance())
		.registerClass(Audio::AudioPCMExtractor::getInstance());
}
