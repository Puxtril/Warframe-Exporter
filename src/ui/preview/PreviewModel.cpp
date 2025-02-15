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
PreviewModel::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs)
{
    auto modelExtractor = WarframeExporter::Model::ModelExtractor::getInstance();

    WarframeExporter::Model::ModelHeaderExternal modelHeaderExt;
    WarframeExporter::Model::ModelBodyExternal modelBodyExt;
    modelExtractor->extractExternal(fileEntry, modelHeaderExt, modelBodyExt);

    WarframeExporter::Model::ModelHeaderInternal headerInt;
	WarframeExporter::Model::ModelBodyInternal bodyInt;
    WarframeExporter::Model::ModelConverter::convertToInternal(modelHeaderExt, modelBodyExt, fileEntry.commonHeader.attributes, std::vector<std::vector<glm::u8vec4>>(), headerInt, bodyInt, WarframeExporter::Model::g_enumMapModel[fileEntry.commonHeader.type]->ensmalleningScale());

    m_modelWidget->loadModel(bodyInt);
}
