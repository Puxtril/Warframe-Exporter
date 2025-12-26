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
#include "model/types/ModelReader109WF.h"
#include "model/types/ModelReader109SF.h"
#include "model/types/ModelReader159.h"
#include "model/types/ModelReader269.h"
#include "model/types/ModelReader27X.h"
#include "model/types/ModelReader281.h"
#include "model/types/ModelReader283.h"
#include "model/types/ModelReader287.h"
#include "model/types/ModelReader289.h"
#include "model/types/ModelReader290.h"
#include "model/types/ModelReader299.h"
#include "model/types/ModelReader300.h"
#include "model/types/ModelReader310.h"
#include "model/types/ModelReader314.h"

#include "model/types/ModelDCMReader101.h"
#include "model/types/ModelDCMReader102.h"
#include "model/types/ModelDCMReader108WF.h"
#include "model/types/ModelDCMReader108SF.h"

#include "model/types/ModelHLODReader102.h"
#include "model/types/ModelHLODReader108WF.h"
#include "model/types/ModelHLODReader108SF.h"


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
		.registerClass(ModelReader109WF::getInstance())
		.registerClass(ModelReader109SF::getInstance())
		.registerClass(ModelReader159::getInstance())
		.registerClass(ModelReader269::getInstance())
		.registerClass(ModelReader27X::getInstance())
		.registerClass(ModelReader281::getInstance())
		.registerClass(ModelReader283::getInstance())
		.registerClass(ModelReader287::getInstance())
		.registerClass(ModelReader289::getInstance())
		.registerClass(ModelReader290::getInstance())
		.registerClass(ModelReader299::getInstance())
		.registerClass(ModelReader300::getInstance())
		.registerClass(ModelReader310::getInstance())
		.registerClass(ModelReader314::getInstance());

	const static EnumMapGame<ModelReader> g_enumMapModelDCM = EnumMapGame<ModelReader>()
		.registerClass(ModelDCMReader101::getInstance())
		.registerClass(ModelDCMReader102::getInstance())
		.registerClass(ModelDCMReader108WF::getInstance())
		.registerClass(ModelDCMReader108SF::getInstance());

	const static EnumMapGame<ModelReader> g_enumMapModelHLOD = EnumMapGame<ModelReader>()
		.registerClass(ModelHLODReader102::getInstance())
		.registerClass(ModelHLODReader108WF::getInstance())
		.registerClass(ModelHLODReader108SF::getInstance());
}