#pragma once

#include "EnumMap.h"
#include "material/types/MaterialReaderBasic.h"
#include "material/types/MaterialReaderHlm3.h"

namespace WarframeExporter::Material
{
	const static EnumMap<MaterialReader> g_enumMapMaterial = EnumMap<MaterialReader>()
		.registerClass(MaterialReaderBasic::getInstance())
        .registerClass(MaterialReaderHlm3::getInstance()
    );
}