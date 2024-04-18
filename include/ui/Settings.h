#pragma once

#include "Extractor.h"
#include "shader/ShaderExportType.h"
#include "texture/TextureExportTypes.h"

#include <filesystem>
#include <QtCore/qobject.h>
#include <string>
#include <QtCore/QSettings>
#include <cstdint>

class UiSettings : public QObject
{
    QSettings m_settings;

    static const inline QString m_cacheWindowsKey = "load/cachePath";
    static const inline QString m_exportPathKey = "load/exportPath";

    static const inline QString m_checkboxExportTextures = "load/export/textures";
    static const inline QString m_checkboxExportModels = "load/export/models";
    static const inline QString m_checkboxExportLevels = "load/export/levels";
    static const inline QString m_checkboxExportMaterials = "load/export/materials";
    static const inline QString m_checkboxExportAudio = "load/export/audio";
    static const inline QString m_checkboxExportShaders = "load/export/shaders";

    static const inline QString m_comboTextureFormat = "load/export/textureformat";
    static const inline QString m_comboShaderFormat = "load/export/shaderformat";

    static const inline QString m_mainWindowGeometry = "geometry/mainwindow";
    static const inline QString m_exporterSplitGeometry = "geometry/splitter";

    static const inline QString m_mainWindowState = "state/mainwindow";
    static const inline QString m_exporterSplitState = "state/splitter";

    UiSettings();
    UiSettings(const UiSettings&) = delete;
    const UiSettings& operator=(const UiSettings&) = delete;

public:
    static UiSettings& getInstance();

    QString getCacheWindowsPath() const;
    QString getExportPath() const;

    bool getExportTextures() const;
    bool getExportModels() const;
    bool getExportLevels() const;
    bool getExportMaterials() const;
    bool getExportAudio() const;
    bool getExportShaders() const;

    void saveMainWindowData(QByteArray geometry, QByteArray state);
    QByteArray getMainWindowGeometry();
    QByteArray getMainWindowState();

    void saveSplitterData(QByteArray geometry, QByteArray state);
    QByteArray getSplitterGeometry();
    QByteArray getSplitterState();

    WarframeExporter::Shader::ShaderExportType getShaderFormat() const;
    WarframeExporter::Texture::TextureExportType getTextureFormat() const;

public slots:
    void setSettings(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        WarframeExporter::Shader::ShaderExportType shaderExportType,
        WarframeExporter::Texture::TextureExportType textureExportType    
    );
};