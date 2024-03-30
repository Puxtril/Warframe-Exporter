#pragma once

#include "EnumMap.h"
#include "shader/types/StandardShaderReader.h"
#include "shader/types/ShaderReader22.h"

namespace WarframeExporter::Shader
{
    const static EnumMap<ShaderReader> g_enumMapShader = EnumMap<ShaderReader>()
        .registerClass(StandardShaderReader::getInstance())
        .registerClass(ShaderReader22::getInstance());
};