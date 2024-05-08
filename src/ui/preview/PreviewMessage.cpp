#include "ui/preview/PreviewMessage.h"
#include "ui/preview/Preview.h"

PreviewMessage*
PreviewMessage::getInstance()
{
    static PreviewMessage instance;
    return &instance;
}

void
PreviewMessage::setMessage(const std::string& msg)
{
    m_widgetText->setText(QString::fromStdString(msg));
}

void
PreviewMessage::setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout)
{
    m_widgetText = new QLabel(parentWidget);
    parentLayout->addWidget(m_widgetText);

    m_widgetText->setObjectName("MessageLabel");
    m_widgetText->setAlignment(Qt::AlignCenter);
    QFont font;
    font.setPointSize(13);
    m_widgetText->setFont(font);
    m_widgetText->hide();
}

void
PreviewMessage::unloadData()
{
}

void
PreviewMessage::hide()
{
    m_widgetText->hide();
}

void
PreviewMessage::show()
{
    m_widgetText->show();
}

void
PreviewMessage::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const WarframeExporter::Ensmallening& ensmalleningData)
{
}
