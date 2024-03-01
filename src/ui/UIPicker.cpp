#include "ui/UIPicker.h"
#include <qnamespace.h>

UiPicker::UiPicker(QObject *parent)
    : QObject(parent)
{
}

void
UiPicker::setupUi(QDialog *WindowPicker)
{
    Ui_WindowPicker::setupUi(WindowPicker);
    WindowPicker->setWindowFlag(Qt::WindowContextHelpButtonHint, true);
}

void
UiPicker::connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter)
{
    QObject::connect(this->LoadButton, &QPushButton::clicked, this, &UiPicker::parsePickerOptions);
    QObject::connect(this, &UiPicker::pickerDone, WindowPicker, &QDialog::hide);
    QObject::connect(this, &UiPicker::pickerDone, mainWindow, &QMainWindow::show);
    QObject::connect(this, &UiPicker::pickerDone, exporter, &UiExporter::setData);
    QObject::connect(this->CacheWindowsBrowse, &QPushButton::clicked, this, &UiPicker::browseCacheWindows);
    QObject::connect(this->ExportPathBrowse, &QPushButton::clicked, this, &UiPicker::browseExportPath);
}

void
UiPicker::parsePickerOptions()
{
    std::string cachePathStr = this->CacheWindowsInput->text().toUtf8().constData();
    std::filesystem::path cachePath(cachePathStr);
    if (!std::filesystem::is_directory(cachePath))
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "Cache.Windows folder is invalid");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    std::string exportPathStr = this->ExportPathInput->text().toUtf8().constData();
    std::filesystem::path exportPath(exportPathStr);
    if (!std::filesystem::is_directory(exportPath))
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "Export folder is invalid");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    int viewTypes = 0;

    if (this->CheckboxViewAudio->isChecked())
        viewTypes |= (int)WarframeExporter::ExtractorType::Audio;
    if (this->CheckboxViewLevels->isChecked())
        viewTypes |= (int)WarframeExporter::ExtractorType::Level;
    if (this->CheckboxViewMaterials->isChecked())
        viewTypes |= (int)WarframeExporter::ExtractorType::Material;
    if (this->CheckboxViewModels->isChecked())
        viewTypes |= (int)WarframeExporter::ExtractorType::Model;
    if (this->CheckboxViewTextures->isChecked())
        viewTypes |= (int)WarframeExporter::ExtractorType::Texture;

    if (viewTypes == 0)
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "No view items selected");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    int exportTypes = 0;

    if (this->CheckboxExportAudio->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Audio;
    if (this->CheckboxExportLevels->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Level;
    if (this->CheckboxExportMaterials->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Material;
    if (this->CheckboxExportModels->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Model;
    if (this->CheckboxExportTextures->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Texture;

    if (exportTypes == 0)
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "No export items selected");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    emit pickerDone(cachePath, exportPath, (WarframeExporter::ExtractorType)viewTypes, (WarframeExporter::ExtractorType)exportTypes);
}

void
UiPicker::browseCacheWindows()
{
    QString directory = QFileDialog::getExistingDirectory(this->CacheWindowsBrowse, tr("Cache.Windows folder"), QDir::rootPath());
    if (!directory.isEmpty())
    {
        this->CacheWindowsInput->setText(directory);
    }
}
void
UiPicker::browseExportPath()
{
    QString directory = QFileDialog::getExistingDirectory(this->ExportPathBrowse, tr("Export folder"), QDir::currentPath());
    if (!directory.isEmpty())
    {
        this->ExportPathInput->setText(directory);
    }
}