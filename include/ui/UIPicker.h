#pragma once

#include "ui/ui_Picker.h"
#include "ui/UIExporter.h"
#include "ui/ui_AdditionalSettings.h"
#include "Extractor.h"
#include "ui/Settings.h"
#include "Meta.h"
#include "shader/ShaderExportType.h"
#include "texture/TextureExportTypes.h"
#include "ExtractOptions.h"

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/QMessageBox>
#include <QtCore/qtmetamacros.h>
#include <QtWidgets/qfiledialog.h>

class UiPicker : public QObject, private Ui_WindowPicker
{
    Q_OBJECT

    QDialog m_additionalSettingsDialog;
    Ui_AdditionalSettings m_additionalSettings;

    QMessageBox m_invalidCacheFolderBox;
    QMessageBox m_invalidExportFolderBox;

public:
    UiPicker(QObject *parent = nullptr);
    virtual ~UiPicker() {}

    void setupUi(QDialog *WindowPicker);
    void connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter);

private:
    void setupMessageBoxes();
    void addShaderFormatOptions();
    void addTextureFormatOptions();
    void addGameOptions();
    void loadSettings();
    void loadVersion();

signals:
    void pickerDone(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        WarframeExporter::Shader::ShaderExportType shaderExportType,
        WarframeExporter::Texture::TextureExportType textureExportType,
        LotusLib::Game game,
        WarframeExporter::ExtractOptions options
    );
    void retryLoadPickerOptions();

public slots:
    void parsePickerOptions();
    void browseCacheWindows();
    void browseExportPath();
    void createExportFolderAndLoad(QAbstractButton *button);
};