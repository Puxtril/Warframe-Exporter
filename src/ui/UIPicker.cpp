#include "ui/UIPicker.h"

UiPicker::UiPicker(QObject *parent)
    : QObject(parent)
{
}

void
UiPicker::setupUi(QDialog *WindowPicker)
{
    Ui_WindowPicker::setupUi(WindowPicker);
}

void
UiPicker::connect(QDialog *WindowPicker, QMainWindow* mainWindow, UiExporter* exporter)
{
    QObject::connect(this->LoadButton, &QPushButton::clicked, this, &UiPicker::sendDataToExporter);
    QObject::connect(this, &UiPicker::pickerDone, WindowPicker, &QDialog::hide);
    QObject::connect(this, &UiPicker::pickerDone, mainWindow, &QMainWindow::show);
    QObject::connect(this, &UiPicker::pickerDone, exporter, &UiExporter::setData);
}

void
UiPicker::sendDataToExporter()
{
    std::string pathStr = this->CacheWindowsInput->text().toUtf8().constData();
    std::filesystem::path pathPath(pathStr);
    if (!std::filesystem::is_directory(pathPath))
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "Specified cache directory is invalid");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    int extractTypes;

    if (this->CheckboxAudio->isChecked())
        extractTypes |= (int)WarframeExporter::ExtractorType::Audio;
    if (this->CheckboxLevels->isChecked())
        extractTypes |= (int)WarframeExporter::ExtractorType::Level;
    if (this->CheckboxMaterials->isChecked())
        extractTypes |= (int)WarframeExporter::ExtractorType::Material;
    if (this->CheckboxModels->isChecked())
        extractTypes |= (int)WarframeExporter::ExtractorType::Model;
    if (this->CheckboxTextures->isChecked())
        extractTypes |= (int)WarframeExporter::ExtractorType::Texture;

    if (extractTypes == 0)
    {
        QMessageBox errBox;
        errBox.critical(nullptr, "Error", "No packages selected");
        errBox.setFixedSize(500, 200);
        errBox.show();
        return;
    }

    emit pickerDone(pathPath, (WarframeExporter::ExtractorType)extractTypes);
}