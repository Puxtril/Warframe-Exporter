#include "ui/preview/PreviewLoading.h"

PreviewLoading*
PreviewLoading::getInstance()
{
    static PreviewLoading instance;
    return &instance;
}

void
PreviewLoading::setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout)
{
    m_widgetText = new QLabel(parentWidget);
    parentLayout->addWidget(m_widgetText);

    m_widgetText->setObjectName("LoadingLabel");
    m_widgetText->setAlignment(Qt::AlignCenter);
    m_widgetText->setText("Loading...");
    QFont font;
    font.setPointSize(13);
    m_widgetText->setFont(font);
    m_widgetText->hide();
}

void
PreviewLoading::unloadData()
{
}

void
PreviewLoading::hide()
{
    m_widgetText->hide();
}

void
PreviewLoading::show()
{
    m_widgetText->show();
}

void
PreviewLoading::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData)
{
}
