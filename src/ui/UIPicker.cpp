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
    addShaderFormatOptions();
    addTextureFormatOptions();
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
UiPicker::addShaderFormatOptions()
{
    this->ShaderFormatCombo->addItem("Binary", WarframeExporter::Shader::ShaderExportType::SHADER_EXPORT_BINARY);
#ifdef WIN32
    this->ShaderFormatCombo->addItem("Decompiled", WarframeExporter::Shader::SHADER_EXPORT_D3DDECOMPILE);
#endif
}

void
UiPicker::addTextureFormatOptions()
{
    this->TextureFormatCombo->addItem("DDS", WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_DDS);
    this->TextureFormatCombo->addItem("PNG",WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_PNG);
    this->TextureFormatCombo->addItem("TGA", WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_TGA);
}

void
UiPicker::loadSettings()
{
    UiSettings& settings = UiSettings::getInstance();

    this->CacheWindowsInput->setText(settings.getCacheWindowsPath());
    this->ExportPathInput->setText(settings.getExportPath());

    this->TextureCheckbox->setCheckState(settings.getExportTextures() ? Qt::Checked : Qt::Unchecked);
    this->ModelCheckbox->setCheckState(settings.getExportModels() ? Qt::Checked : Qt::Unchecked);
    this->LevelCheckbox->setCheckState(settings.getExportLevels() ? Qt::Checked : Qt::Unchecked);
    this->MaterialCheckbox->setCheckState(settings.getExportMaterials() ? Qt::Checked : Qt::Unchecked);
    this->AudioCheckbox->setCheckState(settings.getExportAudio() ? Qt::Checked : Qt::Unchecked);
    this->ShaderCheckbox->setCheckState(settings.getExportShaders() ? Qt::Checked : Qt::Unchecked);

    WarframeExporter::Texture::TextureExportType textureFormat = settings.getTextureFormat();
    switch(textureFormat)
    {
        case WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_DDS:
            this->TextureFormatCombo->setCurrentIndex(0);
            break;
        case WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_PNG:
            this->TextureFormatCombo->setCurrentIndex(1);
            break;
        case WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_TGA:
            this->TextureFormatCombo->setCurrentIndex(2);
            break;
    }

    WarframeExporter::Shader::ShaderExportType shaderFormat = settings.getShaderFormat();
    switch(shaderFormat)
    {
        case WarframeExporter::Shader::ShaderExportType::SHADER_EXPORT_BINARY:
            this->ShaderFormatCombo->setCurrentIndex(0);
            break;
        case WarframeExporter::Shader::ShaderExportType::SHADER_EXPORT_D3DDECOMPILE:
            this->ShaderFormatCombo->setCurrentIndex(1);
            break;
    }
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

    int exportTypes = 0;

    if (this->AudioCheckbox->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Audio;
    if (this->LevelCheckbox->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Level;
    if (this->MaterialCheckbox->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Material;
    if (this->ModelCheckbox->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Model;
    if (this->TextureCheckbox->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Texture;
    if (this->ShaderCheckbox->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Shader;

    if (exportTypes == 0)
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "No export items selected");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    int selectedTextureFormat = this->TextureFormatCombo->itemData(this->TextureFormatCombo->currentIndex()).toInt();
    WarframeExporter::Texture::TextureExportType textureExportType = static_cast<WarframeExporter::Texture::TextureExportType>(selectedTextureFormat);

    int selectedShaderFormat = this->ShaderFormatCombo->itemData(this->ShaderFormatCombo->currentIndex()).toInt();
    WarframeExporter::Shader::ShaderExportType shaderExportType = static_cast<WarframeExporter::Shader::ShaderExportType>(selectedShaderFormat);

    WarframeExporter::Logger::getInstance().setLogProperties(exportPath / "Warframe-Exporter.log", g_logLevel);
    emit pickerDone(cachePath, exportPath, (WarframeExporter::ExtractorType)exportTypes, shaderExportType, textureExportType);
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