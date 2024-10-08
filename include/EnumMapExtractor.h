#pragma once

#include "EnumMap.h"
#include "Extractor.h"
#include "model/ModelExtractor.h"
#include "texture/TextureExtractor.h"
#include "material/MaterialExtractor.h"
#include "level/LevelExtractor.h"
#include "audio/AudioExtractorProxy.h"
#include "shader/ShaderExtractor.h"
#include "landscape/LandscapeExtractor.h"

namespace WarframeExporter
{
	const static EnumMap<Extractor> g_enumMapExtractor = EnumMap<Extractor>()
		.registerClass(Model::ModelExtractor::getInstance())
		.registerClass(Texture::TextureExtractor::getInstance())
		.registerClass(Material::MaterialExtractor::getInstance())
		.registerClass(Level::LevelExtractor::getInstance())
		.registerClass(Audio::AudioExtractorProxy::getInstance())
		.registerClass(Shader::ShaderExtractor::getInstance())
		.registerClass(Landscape::LandscapeExtractor::getInstance());
}
