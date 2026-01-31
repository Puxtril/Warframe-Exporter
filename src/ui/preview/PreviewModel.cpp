#include "ui/preview/PreviewModel.h"

PreviewModel*
PreviewModel::getInstance()
{
    static PreviewModel instance;
    return &instance;
}

void
PreviewModel::setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonsArea, QHBoxLayout* layout)
{
    m_modelWidget = new ModelRenderWidget(parentWidget);
    parentLayout->addWidget(m_modelWidget);

    m_modelWidget->hide();
}

void
PreviewModel::unloadData()
{
}

void
PreviewModel::hide()
{
    m_modelWidget->hide();
}

void
PreviewModel::show()
{
    m_modelWidget->show();
}

void
PreviewModel::setupWidget(LotusLib::FileEntry& fileEntry, const LotusLib::PackageCollection& pkgs, const LotusLib::PackagesBin& pkgsBin)
{
    auto modelExtractor = WarframeExporter::Model::ModelExtractor::getInstance();
    const std::string internalPath = LotusLib::getFullPath(fileEntry.headerNode);

    WarframeExporter::Model::ModelReader* modelReader = modelExtractor->getModelReader(internalPath, fileEntry.commonHeader.type, pkgs.getGame());
	if (modelReader == nullptr)
        throw WarframeExporter::unknown_format_error("Unable to match model " + std::to_string(fileEntry.commonHeader.type) + " with known format");

    WarframeExporter::Model::ModelHeaderExternal modelHeaderExt;
    WarframeExporter::Model::ModelBodyExternal modelBodyExt;
    modelExtractor->extractExternal(modelReader, fileEntry.commonHeader, &fileEntry.header, &fileEntry.body, &fileEntry.footer, pkgs.getGame(), modelHeaderExt, modelBodyExt);

    WarframeExporter::Model::ModelHeaderInternal headerInt;
	WarframeExporter::Model::ModelBodyInternal bodyInt;
    WarframeExporter::Model::ModelConverter::convertToInternal(modelHeaderExt, modelBodyExt, fileEntry.commonHeader.attributes, std::vector<std::vector<glm::u8vec4>>(), headerInt, bodyInt, modelReader->ensmalleningScale(), internalPath);

    m_modelWidget->loadModel(modelBodyExt, bodyInt);
}
