#pragma once

#include "ui/ui_Picker.h"
#include "ui/UIExporter.h"
#include "Extractor.h"
#include "ui/Settings.h"
#include "Meta.h"
#include "shader/ShaderExportType.h"
#include "texture/TextureExportTypes.h"

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/QMessageBox>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qfiledialog.h>

class UiPicker : public QObject, private Ui_WindowPicker
{
    Q_OBJECT

public:
    UiPicker(QObject *parent = nullptr);
    virtual ~UiPicker() {}

    void setupUi(QDialog *WindowPicker);
    void connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter);

private:
    void addShaderFormatOptions();
    void addTextureFormatOptions();
    void loadSettings();
    void loadVersion();

signals:
    void pickerDone(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        WarframeExporter::Shader::ShaderExportType shaderExportType,
        WarframeExporter::Texture::TextureExportType textureExportType    
    );

public slots:
    void parsePickerOptions();
    void browseCacheWindows();
    void browseExportPath();
};