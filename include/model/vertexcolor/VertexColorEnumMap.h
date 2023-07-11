#pragma once

#include "EnumMap.h"
#include "model/vertexcolor/VertexColorReader.h"
#include "model/vertexcolor/types/VertexColorReader110.h"

namespace WarframeExporter::Model::VertexColor
{
	const static EnumMap<VertexColorReader> g_enumMapVertexColor = EnumMap<VertexColorReader>()
		.registerClass(VertexColorReader110::getInstance());
}