#pragma once

#include "EnumMap.h"
#include "model/types/ModelReader86.h"
#include "model/types/ModelReader96.h"
#include "model/types/ModelReader99.h"
#include "model/types/ModelReader101.h"
#include "model/types/ModelReader102.h"
#include "model/types/ModelReader103.h"
#include "model/types/ModelReader159.h"
#include "model/types/ModelReader160.h"
#include "model/types/ModelReader269.h"
#include "model/types/ModelReader272.h"
#include "model/types/ModelReader289.h"
#include "model/types/ModelReader290.h"

namespace WarframeExporter::Model
{
	const static EnumMap<ModelReader> g_enumMapModel = EnumMap<ModelReader>()
		.registerClass(ModelReader86::getInstance())
		.registerClass(ModelReader96::getInstance())
		.registerClass(ModelReader99::getInstance())
		.registerClass(ModelReader101::getInstance())
		.registerClass(ModelReader102::getInstance())
		.registerClass(ModelReader103::getInstance())
		.registerClass(ModelReader159::getInstance())
		.registerClass(ModelReader160::getInstance())
		.registerClass(ModelReader269::getInstance())
		.registerClass(ModelReader272::getInstance())
		.registerClass(ModelReader289::getInstance())
		.registerClass(ModelReader290::getInstance());
}