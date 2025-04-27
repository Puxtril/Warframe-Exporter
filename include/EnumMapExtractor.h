#pragma once

#include "EnumMapExtractor.h"
#include "model/ModelExtractor.h"
#include "texture/TextureExtractor.h"
#include "material/MaterialExtractor.h"
#include "level/LevelExtractor.h"
#include "audio/AudioExtractorProxy.h"
#include "shader/ShaderExtractor.h"
#include "landscape/LandscapeExtractor.h"
#include "levelstatic/LevelStaticExtractor.h"

namespace WarframeExporter
{
    // All new extractors must be registered here.
    // This library relies heavily on this variable.
    const static EnumMapExtractor g_enumMapExtractor = EnumMapExtractor()
        .registerClass(Model::ModelExtractor::getInstance())
        .registerClass(Texture::TextureExtractor::getInstance())
        .registerClass(Material::MaterialExtractor::getInstance())
        .registerClass(Level::LevelExtractor::getInstance())
        .registerClass(Audio::AudioExtractorProxy::getInstance())
        .registerClass(Shader::ShaderExtractor::getInstance())
        .registerClass(Landscape::LandscapeExtractor::getInstance())
        .registerClass(LevelStatic::LevelStaticExtractor::getInstance());
};