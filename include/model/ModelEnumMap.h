#pragma once

#include "EnumMapGame.h"

#include "model/types/ModelReader86.h"
#include "model/types/ModelReader96.h"
#include "model/types/ModelReader99.h"
#include "model/types/ModelReader102WF.h"
#include "model/types/ModelReader102SF.h"
#include "model/types/ModelReader103.h"
#include "model/types/ModelReader105WF.h"
#include "model/types/ModelReader105SF.h"
#include "model/types/ModelReader106.h"
#include "model/types/ModelReader109.h"
#include "model/types/ModelReader159.h"
#include "model/types/ModelReader269.h"
#include "model/types/ModelReader27X.h"
#include "model/types/ModelReader281.h"
#include "model/types/ModelReader289.h"
#include "model/types/ModelReader290.h"
#include "model/types/ModelReader299.h"
#include "model/types/ModelReader300.h"

#include "model/types/ModelDCMReader101.h"
#include "model/types/ModelDCMReader102.h"
#include "model/types/ModelDCMReader108.h"

#include "model/types/ModelHLODReader102.h"
#include "model/types/ModelHLODReader108.h"


namespace WarframeExporter::Model
{
	const static EnumMapGame<ModelReader> g_enumMapModel = EnumMapGame<ModelReader>()
		.registerClass(ModelReader86::getInstance())
		.registerClass(ModelReader96::getInstance())
		.registerClass(ModelReader99::getInstance())
		.registerClass(ModelReader102WF::getInstance())
		.registerClass(ModelReader102SF::getInstance())
		.registerClass(ModelReader103::getInstance())
		.registerClass(ModelReader105WF::getInstance())
		.registerClass(ModelReader105SF::getInstance())
		.registerClass(ModelReader106::getInstance())
		.registerClass(ModelReader109::getInstance())
		.registerClass(ModelReader159::getInstance())
		.registerClass(ModelReader269::getInstance())
		.registerClass(ModelReader27X::getInstance())
		.registerClass(ModelReader281::getInstance())
		.registerClass(ModelReader289::getInstance())
		.registerClass(ModelReader290::getInstance())
		.registerClass(ModelReader299::getInstance())
		.registerClass(ModelReader300::getInstance());

	const static EnumMapGame<ModelReader> g_enumMapModelDCM = EnumMapGame<ModelReader>()
		.registerClass(ModelDCMReader101::getInstance())
		.registerClass(ModelDCMReader102::getInstance())
		.registerClass(ModelDCMReader108::getInstance());

	const static EnumMapGame<ModelReader> g_enumMapModelHLOD = EnumMapGame<ModelReader>()
		.registerClass(ModelHLODReader102::getInstance())
		.registerClass(ModelHLODReader108::getInstance());
}