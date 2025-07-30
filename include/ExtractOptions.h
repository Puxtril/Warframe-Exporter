#pragma once

#include "shader/ShaderExportType.h"
#include "texture/TextureExportTypes.h"

namespace WarframeExporter
{
    struct ExtractOptions
    {
        bool dryRun = false;
        bool filterUiFiles = false;
        bool extractVertexColors = false;
        WarframeExporter::Texture::TextureExportType textureExportType;
        WarframeExporter::Shader::ShaderExportType shaderExportType;
    };
}