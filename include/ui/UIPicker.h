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
#include "LotusUtils.h"

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

    QMessageBox m_chosenGameMessage;

public:
    UiPicker(QObject *parent = nullptr);
    virtual ~UiPicker() {}

    void setupUi(QDialog *WindowPicker);
    void connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter);
    void loadSettings();

private:
    void addComboBoxOptions();
    void loadVersion();
    bool createExportFolder();
    void cachePathUpdated(const QString& newPath);

signals:
    void pickerDone(
        std::filesystem::path cachePath,
        std::filesystem::path exportPath,
        WarframeExporter::ExtractorType extractTypes,
        LotusLib::Game game,
        WarframeExporter::ExtractOptions options
    );

public slots:
    void parsePickerOptions();
    void browseCacheWindows();
    void browseExportPath();
};