#pragma once

#include "EnumMap.h"
#include "Extractor.h"
#include "model/ExtractorModel.h"
#include "texture/ExtractorTexture.h"
#include "material/ExtractorMaterial.h"

namespace WarframeExporter
{
	const static EnumMap<Extractor> g_enumMapExtractor = EnumMap<Extractor>()
		.registerClass(Model::ExtractorModel::getInstance())
		.registerClass(Texture::ExtractorTexture::getInstance())
		.registerClass(Material::ExtractorMaterial::getInstance());
}