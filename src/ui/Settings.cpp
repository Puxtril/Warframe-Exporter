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
UiSettings::getViewTextures() const
{
    return m_settings.value(m_checkboxViewTextures).toBool();
}

bool
UiSettings::getViewModels() const
{
    return m_settings.value(m_checkboxViewModels).toBool();
}

bool
UiSettings::getViewLevels() const
{
    return m_settings.value(m_checkboxViewLevels).toBool();
}

bool
UiSettings::getViewMaterials() const
{
    return m_settings.value(m_checkboxViewMaterials).toBool();
}

bool
UiSettings::getViewAudio() const
{
    return m_settings.value(m_checkboxViewAudio).toBool();
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

void
UiSettings::setSettings(std::filesystem::path cachePath, std::filesystem::path exportPath, WarframeExporter::ExtractorType viewTypes, WarframeExporter::ExtractorType extractTypes)
{
    QString qCachePath = QString(cachePath.c_str());
    m_settings.setValue(m_cacheWindowsKey, qCachePath);

    QString qExportPath = QString(exportPath.c_str());
    m_settings.setValue(m_exportPathKey, qExportPath);

    m_settings.setValue(m_checkboxViewTextures, ((int)viewTypes & (int)WarframeExporter::ExtractorType::Texture) > 0);
    m_settings.setValue(m_checkboxViewModels, ((int)viewTypes & (int)WarframeExporter::ExtractorType::Model) > 0);
    m_settings.setValue(m_checkboxViewLevels, ((int)viewTypes & (int)WarframeExporter::ExtractorType::Level) > 0);
    m_settings.setValue(m_checkboxViewMaterials, ((int)viewTypes & (int)WarframeExporter::ExtractorType::Material) > 0);
    m_settings.setValue(m_checkboxViewAudio, ((int)viewTypes & (int)WarframeExporter::ExtractorType::Audio) > 0);

    m_settings.setValue(m_checkboxExportTextures, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Texture) > 0);
    m_settings.setValue(m_checkboxExportModels, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Model) > 0);
    m_settings.setValue(m_checkboxExportLevels, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Level) > 0);
    m_settings.setValue(m_checkboxExportMaterials, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Material) > 0);
    m_settings.setValue(m_checkboxExportAudio, ((int)extractTypes & (int)WarframeExporter::ExtractorType::Audio) > 0);
}
