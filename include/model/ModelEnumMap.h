#pragma once

#include "EnumMap.h"
#include "model/types/ModelReader96.hpp"
#include "model/types/ModelReader99.hpp"
#include "model/types/ModelReader103.hpp"
#include "model/types/ModelReader159.hpp"
#include "model/types/ModelReader269.hpp"
#include "model/types/ModelReader272.hpp"

namespace WarframeExporter::Model
{
	const static EnumMap<ModelReader> g_enumMapModel = EnumMap<ModelReader>()
		.registerClass(ModelReader96::getInstance())
		.registerClass(ModelReader99::getInstance())
		.registerClass(ModelReader103::getInstance())
		.registerClass(ModelReader159::getInstance())
		.registerClass(ModelReader269::getInstance())
		.registerClass(ModelReader272::getInstance());
}