#pragma once

#include "EnumMap.h"
#include "shader/types/StandardShaderReader.h"

namespace WarframeExporter::Shader
{
    const static EnumMap<ShaderReader> g_enumMapShader = EnumMap<ShaderReader>()
        .registerClass(StandardShaderReader::getInstance());
};