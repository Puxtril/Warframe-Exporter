#pragma once

#include "shader/ShaderExportType.h"
#include "texture/TextureExportTypes.h"
#include "level/LevelExtractOptions.h"
#include "material/MaterialExtractOptions.h"

namespace WarframeExporter
{
    struct ExtractOptions
    {
        bool dryRun = false;
        bool filterUiFiles = false;
        bool extractVertexColors = false;
        WarframeExporter::Texture::TextureExportType textureExportType = WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_PNG;
        WarframeExporter::Shader::ShaderExportType shaderExportType = WarframeExporter::Shader::ShaderExportType::SHADER_EXPORT_BINARY;
        WarframeExporter::Level::LevelHlodExtractMode levelHlodExtractMode = WarframeExporter::Level::LevelHlodExtractMode::IGNORE_HLOD;
        WarframeExporter::Material::MaterialExtractType materialExtractMode = WarframeExporter::Material::MaterialExtractType::TXT;
    };
}