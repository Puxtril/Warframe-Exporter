#pragma once

#include "Extractor.h"

#include <filesystem>
#include <qobject.h>
#include <string>
#include <QSettings>

class UiSettings : public QObject
{
    QSettings m_settings;

    static const inline QString m_cacheWindowsKey = "load/cachePath";
    static const inline QString m_exportPathKey = "load/exportPath";
    
    static const inline QString m_checkboxViewTextures = "load/view/textures";
    static const inline QString m_checkboxViewModels = "load/view/models";
    static const inline QString m_checkboxViewLevels = "load/view/levels";
    static const inline QString m_checkboxViewMaterials = "load/view/materials";
    static const inline QString m_checkboxViewAudio = "load/view/audio";

    static const inline QString m_checkboxExportTextures = "load/export/textures";
    static const inline QString m_checkboxExportModels = "load/export/models";
    static const inline QString m_checkboxExportLevels = "load/export/levels";
    static const inline QString m_checkboxExportMaterials = "load/export/materials";
    static const inline QString m_checkboxExportAudio = "load/export/audio";

    UiSettings();
    UiSettings(const UiSettings&) = delete;
    const UiSettings& operator=(const UiSettings&) = delete;

public:
    static UiSettings& getInstance();

    QString getCacheWindowsPath() const;
    QString getExportPath() const;

    bool getViewTextures() const;
    bool getViewModels() const;
    bool getViewLevels() const;
    bool getViewMaterials() const;
    bool getViewAudio() const;

    bool getExportTextures() const;
    bool getExportModels() const;
    bool getExportLevels() const;
    bool getExportMaterials() const;
    bool getExportAudio() const;

public slots:
    void setSettings(std::filesystem::path cachePath, std::filesystem::path exportPath, WarframeExporter::ExtractorType viewTypes, WarframeExporter::ExtractorType extractTypes);
};