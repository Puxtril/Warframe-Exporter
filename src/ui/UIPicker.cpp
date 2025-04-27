#include "ui/UIPicker.h"

UiPicker::UiPicker(QObject *parent)
    : QObject(parent)
{
}

void
UiPicker::setupUi(QDialog *WindowPicker)
{
    m_additionalSettings.setupUi(&m_additionalSettingsDialog);
    
    Ui_WindowPicker::setupUi(WindowPicker);
    WindowPicker->setWindowFlag(Qt::WindowContextHelpButtonHint, true);
    setupMessageBoxes();
    addShaderFormatOptions();
    addTextureFormatOptions();
    addGameOptions();
    loadSettings();
    loadVersion();
}

void
UiPicker::connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter)
{
    QObject::connect(this->AdditionalSettingsButton, &QPushButton::clicked, this, &UiPicker::additionalSettingsClicked);
    QObject::connect(m_additionalSettings.buttonBox, &QDialogButtonBox::accepted, this, &UiPicker::additionalSettingsClosed);
    QObject::connect(m_additionalSettings.buttonBox, &QDialogButtonBox::rejected, this, &UiPicker::additionalSettingsCancelled);

    QObject::connect(this->LoadButton, &QPushButton::clicked, this, &UiPicker::parsePickerOptions);
    QObject::connect(this, &UiPicker::pickerDone, WindowPicker, &QDialog::hide);
    QObject::connect(this, &UiPicker::pickerDone, mainWindow, &QMainWindow::show);
    QObject::connect(this, &UiPicker::pickerDone, exporter, &UiExporter::setData);
    QObject::connect(this, &UiPicker::pickerDone, &UiSettings::getInstance(), &UiSettings::setSettings);
    QObject::connect(this->CacheWindowsBrowse, &QPushButton::clicked, this, &UiPicker::browseCacheWindows);
    QObject::connect(this->ExportPathBrowse, &QPushButton::clicked, this, &UiPicker::browseExportPath);

    QObject::connect(&this->m_invalidExportFolderBox, &QMessageBox::buttonClicked, this, &UiPicker::createExportFolderAndLoad);
    QObject::connect(this, &UiPicker::retryLoadPickerOptions, this, &UiPicker::parsePickerOptions);
}

void
UiPicker::setupMessageBoxes()
{
    m_invalidCacheFolderBox.setWindowTitle("Error");
    m_invalidCacheFolderBox.setText("Cache.Windows folder is invalid");
    m_invalidCacheFolderBox.setIcon(QMessageBox::Critical);
    m_invalidCacheFolderBox.setFixedSize(500, 200);
    m_invalidCacheFolderBox.setModal(true);
    m_invalidCacheFolderBox.setVisible(false);

    m_invalidExportFolderBox.setText("Export folder doesn't exist, create?");
    m_invalidExportFolderBox.setWindowTitle("Error");
    m_invalidExportFolderBox.setIcon(QMessageBox::Question);
    m_invalidExportFolderBox.setFixedSize(500, 200);
    m_invalidExportFolderBox.setModal(true);
    m_invalidExportFolderBox.setVisible(false);

    m_invalidExportFolderBox.addButton(QMessageBox::Cancel);
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
UiPicker::addGameOptions()
{
    this->GamePickerCombo->addItem("Warframe", (int)LotusLib::Game::WARFRAME);
    this->GamePickerCombo->addItem("Soulframe", (int)LotusLib::Game::SOULFRAME);
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

    LotusLib::Game game = settings.getGame();
    switch(game)
    {
        case LotusLib::Game::UNKNOWN:
        case LotusLib::Game::WARFRAME:
            this->GamePickerCombo->setCurrentIndex(0);
            break;
        case LotusLib::Game::SOULFRAME:
            this->GamePickerCombo->setCurrentIndex(1);
            break;
        case LotusLib::Game::WARFRAME_PE:
            this->GamePickerCombo->setCurrentIndex(2);
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
        m_invalidCacheFolderBox.show();
        return;
    }

    std::string exportPathStr = this->ExportPathInput->text().toUtf8().constData();
    std::filesystem::path exportPath(exportPathStr);
    if (!std::filesystem::is_directory(exportPath))
    {
        m_invalidExportFolderBox.show();
        return;
    }

    int exportTypes = 0;

    if (this->AudioCheckbox->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Audio;
    if (this->LevelCheckbox->isChecked())
        exportTypes |= (int)WarframeExporter::ExtractorType::Level | (int)WarframeExporter::ExtractorType::LevelStatic;
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

    int selectedGame = this->GamePickerCombo->itemData(this->GamePickerCombo->currentIndex()).toInt();
    LotusLib::Game gameType = static_cast<LotusLib::Game>(selectedGame);

    WarframeExporter::Logger::getInstance().setLogProperties(exportPath / "Warframe-Exporter.log", g_logLevel);
    LotusLib::Logger::setLogProperties(spdlog::level::info);
    emit pickerDone(cachePath, exportPath, (WarframeExporter::ExtractorType)exportTypes, shaderExportType, textureExportType, gameType);
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

void
UiPicker::createExportFolderAndLoad(QAbstractButton *button)
{
    if (m_invalidExportFolderBox.standardButton(button) != QMessageBox::Ok)
        return;

    std::string exportPathStr = this->ExportPathInput->text().toUtf8().constData();
    std::filesystem::path exportPath(exportPathStr);
    try
    {
        std::filesystem::create_directories(exportPath);
    }
    catch (std::exception& ex)
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error creating folder", ex.what());
        return;
    }

    emit retryLoadPickerOptions();
}

void
UiPicker::additionalSettingsClicked()
{
    Qt::CheckState isChecked = UiSettings::getInstance().getFilterFiles() ? Qt::CheckState::Checked : Qt::CheckState::Unchecked;
    m_additionalSettings.FilterFilesCheckbox->setCheckState(isChecked);

    m_additionalSettingsDialog.setModal(Qt::ApplicationModal);
    m_additionalSettingsDialog.show();
}

void
UiPicker::additionalSettingsClosed()
{
    m_additionalSettingsDialog.hide();

    UiSettings::getInstance().setFilterFiles(m_additionalSettings.FilterFilesCheckbox->isChecked());
}

void
UiPicker::additionalSettingsCancelled()
{
    m_additionalSettingsDialog.hide();
}