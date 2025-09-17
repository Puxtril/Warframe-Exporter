#include "ui/preview/PreviewUnsupported.h"

PreviewUnsupported*
PreviewUnsupported::getInstance()
{
    static PreviewUnsupported instance;
    return &instance;
}

void
PreviewUnsupported::setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonsArea, QHBoxLayout* layout)
{
    m_widgetText = new QLabel(parentWidget);
    parentLayout->addWidget(m_widgetText);

    m_widgetText->setObjectName("LoadingLabel");
    m_widgetText->setAlignment(Qt::AlignCenter);
    m_widgetText->setText("Preview Not Supported!");
    QFont font;
    font.setPointSize(13);
    m_widgetText->setFont(font);
    m_widgetText->hide();
}

void
PreviewUnsupported::unloadData()
{
}

void
PreviewUnsupported::hide()
{
    m_widgetText->hide();
}

void
PreviewUnsupported::show()
{
    m_widgetText->show();
}

void
PreviewUnsupported::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs)
{
}
