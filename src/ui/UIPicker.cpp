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
    addComboBoxOptions();
    loadVersion();

    // When `CacheWindowsInput` is updated, this will show
    this->GameInfoButton->hide();
}

void
UiPicker::connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter)
{
    QObject::connect(this->AdditionalSettingsButton, &QPushButton::clicked, &m_additionalSettingsDialog, &QDialog::show);
    QObject::connect(m_additionalSettings.buttonBox, &QDialogButtonBox::accepted, &m_additionalSettingsDialog, &QDialog::hide);
    QObject::connect(m_additionalSettings.buttonBox, &QDialogButtonBox::rejected, &m_additionalSettingsDialog, &QDialog::hide);

    QObject::connect(this->CacheWindowsInput, &QLineEdit::textChanged, this, &UiPicker::cachePathUpdated);
    QObject::connect(this->GameInfoButton, &QPushButton::clicked, &this->m_chosenGameMessage, &QMessageBox::show);

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
    m_chosenGameMessage.setFixedSize(500, 200);
    m_chosenGameMessage.setModal(true);
    m_chosenGameMessage.setVisible(false);

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
UiPicker::addComboBoxOptions()
{
    this->ShaderFormatCombo->addItem("Binary", WarframeExporter::Shader::ShaderExportType::SHADER_EXPORT_BINARY);
#ifdef WIN32
    this->ShaderFormatCombo->addItem("Decompiled", WarframeExporter::Shader::SHADER_EXPORT_D3DDECOMPILE);
#endif

    this->TextureFormatCombo->addItem("DDS", WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_DDS);
    this->TextureFormatCombo->addItem("PNG",WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_PNG);
    this->TextureFormatCombo->addItem("TGA", WarframeExporter::Texture::TextureExportType::TEXTURE_EXPORT_TGA);

    m_additionalSettings.LevelHlodExportCombo->addItem("Ignore", WarframeExporter::Level::LevelHlodExtractMode::IGNORE_HLOD);
    m_additionalSettings.LevelHlodExportCombo->addItem("Include", WarframeExporter::Level::LevelHlodExtractMode::INCLUDE_HLOD);
    m_additionalSettings.LevelHlodExportCombo->addItem("Only", WarframeExporter::Level::LevelHlodExtractMode::ONLY_HLOD);
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

    WarframeExporter::ExtractOptions options = settings.loadOptions();
    this->TextureFormatCombo->setCurrentIndex(this->TextureFormatCombo->findData(options.textureExportType));
    this->ShaderFormatCombo->setCurrentIndex(this->ShaderFormatCombo->findData(options.shaderExportType));
    m_additionalSettings.LevelHlodExportCombo->setCurrentIndex(m_additionalSettings.LevelHlodExportCombo->findData(options.levelHlodExtractMode));

    m_additionalSettings.FilterFilesCheckbox->setCheckState(options.filterUiFiles ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    m_additionalSettings.ExtractVertexColorsCheckbox->setCheckState(options.extractVertexColors ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
}

void
UiPicker::loadVersion()
{
    QString version = std::string(g_version).c_str();
    this->VersionLabel->setText(version);
}

void
UiPicker::cachePathUpdated(const QString& newPath)
{
    LotusLib::Game newGame = LotusLib::guessGame(newPath.toStdString());

    bool disableLoadButton = true;
    QIcon::ThemeIcon buttonIcon;
    QMessageBox::Icon msgBoxIcon;
    QString msgBoxMsg;

    switch (newGame)
    {
        case LotusLib::Game::STARTREK:
        {
            buttonIcon = QIcon::ThemeIcon::DialogError;
            msgBoxIcon = QMessageBox::Icon::Critical;
            msgBoxMsg = "Star Trek is currently unsupported. It can be supported, but noone has showed interest so it's currently backlogged. Why do you have these files? This isn't a good game.";
            disableLoadButton = true;
            break;
        }
        case LotusLib::Game::DARKNESSII:
        {
            buttonIcon = QIcon::ThemeIcon::DialogError;
            msgBoxIcon = QMessageBox::Icon::Critical;
            msgBoxMsg = "Darkness II is currently unsupported. It can be supported, but noone has showed interest so it's currently backlogged. I'd be surprised if anyone actually sees this message.";
            disableLoadButton = true;
            break;
        }
        case LotusLib::Game::WARFRAME_PE:
        {
            buttonIcon = QIcon::ThemeIcon::DialogWarning;
            msgBoxIcon = QMessageBox::Icon::Warning;
            msgBoxMsg = "Warframe pre-Ensmallening (anything before 2022) is not well supported. Expect crashes and lots of missing functionality. There's lots of development work needed here.";
            disableLoadButton = false;
            break;
        }
        case LotusLib::Game::WARFRAME:
        {
            buttonIcon = QIcon::ThemeIcon::DialogInformation;
            msgBoxIcon = QMessageBox::Icon::Information;
            msgBoxMsg = "Warframe is typically well-supported on the latest version. However, game updates are likely to break extractor functionality.";
            disableLoadButton = false;
            break;
        }
        case LotusLib::Game::SOULFRAME:
        {
            buttonIcon = QIcon::ThemeIcon::DialogWarning;
            msgBoxIcon = QMessageBox::Icon::Warning;
            msgBoxMsg = "Soulframe Preludes 8 is the only supported version (currently). Newer versions may successfully load, but many files will be unsupported.";
            disableLoadButton = false;
            break;
        }
        case LotusLib::Game::UNKNOWN:
        default:
        {
            buttonIcon = QIcon::ThemeIcon::DialogQuestion;
            msgBoxIcon = QMessageBox::Icon::Question;
            msgBoxMsg = "Unknown game. Try selecting another Cache.Windows folder!";
            disableLoadButton = true;
            break;
        }
    }

    this->LoadButton->setDisabled(disableLoadButton);
    this->GameInfoButton->show();
    this->GameInfoButton->setIcon(QIcon::fromTheme(buttonIcon).pixmap(100, 100));
    m_chosenGameMessage.setIcon(msgBoxIcon);
    m_chosenGameMessage.setText(msgBoxMsg);

    std::string newTitle = LotusLib::gameToString(newGame) + " Information";
    m_chosenGameMessage.setWindowTitle(newTitle.c_str());
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

    WarframeExporter::ExtractOptions options;

    int selectedTextureFormat = this->TextureFormatCombo->itemData(this->TextureFormatCombo->currentIndex()).toInt();
    options.textureExportType = static_cast<WarframeExporter::Texture::TextureExportType>(selectedTextureFormat);

    int selectedShaderFormat = this->ShaderFormatCombo->itemData(this->ShaderFormatCombo->currentIndex()).toInt();
    options.shaderExportType = static_cast<WarframeExporter::Shader::ShaderExportType>(selectedShaderFormat);

    int levelHlodExtractMode = m_additionalSettings.LevelHlodExportCombo->itemData(m_additionalSettings.LevelHlodExportCombo->currentIndex()).toInt();
    options.levelHlodExtractMode = static_cast<WarframeExporter::Level::LevelHlodExtractMode>(levelHlodExtractMode);

    options.filterUiFiles = m_additionalSettings.FilterFilesCheckbox->isChecked();
    options.extractVertexColors = m_additionalSettings.ExtractVertexColorsCheckbox->isChecked();

    LotusLib::Game game = LotusLib::guessGame(cachePath);
    WarframeExporter::Logger::getInstance().info("Setting game to " + LotusLib::gameToString(game));

    WarframeExporter::Logger::getInstance().setLogProperties(exportPath / "Warframe-Exporter.log", g_logLevel);
    LotusLib::Logger::setLogProperties(spdlog::level::info);
    emit pickerDone(cachePath, exportPath, (WarframeExporter::ExtractorType)exportTypes, game, options);
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
