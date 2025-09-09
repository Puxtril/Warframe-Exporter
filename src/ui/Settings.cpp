#include "ui/Settings.h"

UiSettings::UiSettings()
    : m_settings("Puxtril", "Warframe Extractor")
{
}

UiSettings&
UiSettings::getInstance()
{
    static UiSettings instance;
    return instance;
}

QString
UiSettings::getCacheWindowsPath() const
{
    return m_settings.value(m_cacheWindowsKey).toString();
}

QString
UiSettings::getExportPath() const
{
    return m_settings.value(m_exportPathKey).toString();
}

bool
UiSettings::getExportTextures() const
{
    return m_settings.value(m_checkboxExportTextures).toBool();
}

bool
UiSettings::getExportModels() const
{
    return m_settings.value(m_checkboxExportModels).toBool();
}

bool
UiSettings::getExportLevels() const
{
    return m_settings.value(m_checkboxExportLevels).toBool();
}

bool
UiSettings::getExportMaterials() const
{
    return m_settings.value(m_checkboxExportMaterials).toBool();
}

bool
UiSettings::getExportAudio() const
{
    return m_settings.value(m_checkboxExportAudio).toBool();
}

bool
UiSettings::getExportShaders() const
{
    return m_settings.value(m_checkboxExportShaders).toBool();
}

WarframeExporter::ExtractOptions
UiSettings::loadOptions()
{
    WarframeExporter::ExtractOptions options;

    options.filterUiFiles = m_settings.value(m_filterExportTypes).isValid() ? m_settings.value(m_filterExportTypes).toBool() : options.filterUiFiles;
    options.extractVertexColors = m_settings.value(m_extractVertexColors).isValid() ? m_settings.value(m_extractVertexColors).toBool() : options.extractVertexColors;

    int value = m_settings.value(m_comboShaderFormat).isValid() ? m_settings.value(m_comboShaderFormat).toInt() : options.shaderExportType;
    options.shaderExportType = static_cast<WarframeExporter::Shader::ShaderExportType>(value);

    value = m_settings.value(m_comboTextureFormat).isValid() ? m_settings.value(m_comboTextureFormat).toInt() : options.textureExportType;
    options.textureExportType = static_cast<WarframeExporter::Texture::TextureExportType>(value);

    value = m_settings.value(m_levelHlodExportType).isValid() ? m_settings.value(m_levelHlodExportType).toInt() : options.levelHlodExtractMode;
    options.levelHlodExtractMode = static_cast<WarframeExporter::Level::LevelHlodExtractMode>(value);
    
    return options;
}

void
UiSettings::saveMainWindowData(QSize size)
{
    m_settings.setValue(m_mainWindowSize, size);
}

QSize
UiSettings::getMainWindowSize()
{
    return m_settings.value(m_mainWindowSize).toSize();
}

void
UiSettings::saveSplitterData(QByteArray geometry, QByteArray state)
{
    m_settings.setValue(m_exporterSplitGeometry, geometry);
    m_settings.setValue(m_exporterSplitState, state);
}

QByteArray
UiSettings::getSplitterGeometry()
{
    return m_settings.value(m_exporterSplitGeometry).toByteArray();
}

QByteArray
UiSettings::getSplitterState()
{
    return m_settings.value(m_exporterSplitState).toByteArray();
}

void
UiSettings::setSettings(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        LotusLib::Game game,
        WarframeExporter::ExtractOptions options
    )
{
    QString qCachePath = QString(cachePath.string().c_str());
    m_settings.setValue(m_cacheWindowsKey, qCachePath);

    QString qExportPath = QString(exportPath.string().c_str());
    m_settings.setValue(m_exportPathKey, qExportPath);

    m_settings.setValue(m_checkboxExportTextures, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Texture) > 0);
    m_settings.setValue(m_checkboxExportModels, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Model) > 0);
    m_settings.setValue(m_checkboxExportLevels, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Level) > 0);
    m_settings.setValue(m_checkboxExportMaterials, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Material) > 0);
    m_settings.setValue(m_checkboxExportAudio, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Audio) > 0);
    m_settings.setValue(m_checkboxExportShaders, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Shader) > 0);

    m_settings.setValue(m_comboShaderFormat, (int)options.shaderExportType);
    m_settings.setValue(m_comboTextureFormat, (int)options.textureExportType);

    m_settings.setValue(m_filterExportTypes, options.filterUiFiles);
    m_settings.setValue(m_extractVertexColors, options.extractVertexColors);
    m_settings.setValue(m_levelHlodExportType, options.levelHlodExtractMode);
}
