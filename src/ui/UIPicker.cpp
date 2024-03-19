#include "ui/UIPicker.h"

UiPicker::UiPicker(QObject *parent)
    : QObject(parent)
{
}

void
UiPicker::setupUi(QDialog *WindowPicker)
{
    Ui_WindowPicker::setupUi(WindowPicker);
    WindowPicker->setWindowFlag(Qt::WindowContextHelpButtonHint, true);
    loadSettings();
    loadVersion();
}

void
UiPicker::connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter)
{
    QObject::connect(this->LoadButton, &QPushButton::clicked, this, &UiPicker::parsePickerOptions);
    QObject::connect(this, &UiPicker::pickerDone, WindowPicker, &QDialog::hide);
    QObject::connect(this, &UiPicker::pickerDone, mainWindow, &QMainWindow::show);
    QObject::connect(this, &UiPicker::pickerDone, exporter, &UiExporter::setData);
    QObject::connect(this, &UiPicker::pickerDone, &UiSettings::getInstance(), &UiSettings::setSettings);
    QObject::connect(this->CacheWindowsBrowse, &QPushButton::clicked, this, &UiPicker::browseCacheWindows);
    QObject::connect(this->ExportPathBrowse, &QPushButton::clicked, this, &UiPicker::browseExportPath);
}

void
UiPicker::loadSettings()
{
    UiSettings& settings = UiSettings::getInstance();

    this->CacheWindowsInput->setText(settings.getCacheWindowsPath());
    this->ExportPathInput->setText(settings.getExportPath());
    
    this->CheckboxViewTextures->setCheckState(settings.getViewTextures() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxViewModels->setCheckState(settings.getViewModels() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxViewLevels->setCheckState(settings.getViewLevels() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxViewMaterials->setCheckState(settings.getViewMaterials() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxViewAudio->setCheckState(settings.getViewAudio() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxViewShaders->setCheckState(settings.getViewShaders() ? Qt::Checked : Qt::Unchecked);

    this->CheckboxExportTextures->setCheckState(settings.getExportTextures() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxExportModels->setCheckState(settings.getExportModels() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxExportLevels->setCheckState(settings.getExportLevels() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxExportMaterials->setCheckState(settings.getExportMaterials() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxExportAudio->setCheckState(settings.getExportAudio() ? Qt::Checked : Qt::Unchecked);
    this->CheckboxExportShaders->setCheckState(settings.getExportShaders() ? Qt::Checked : Qt::Unchecked);
}

void
UiPicker::loadVersion()
{
    QString version = std::string(g_version).c_str();
    this->VersionLabel->setText(version);
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
    if (this->CheckboxViewShaders->isChecked())
        viewTypes |= (int)WarframeExporter::ExtractorType::Shader;

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
    if (this->CheckboxExportShaders->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Shader;

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