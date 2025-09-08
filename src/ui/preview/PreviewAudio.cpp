#include "ui/preview/PreviewAudio.h"

PreviewAudio*
PreviewAudio::getInstance()
{
    static PreviewAudio instance;
    return &instance;
}

void
PreviewAudio::setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonsArea, QHBoxLayout* layout)
{
    m_audioPlaybackWidget.setup();
    createUi(parentWidget, parentLayout);
    m_audioPlaybackWidget.connectToWidgets(m_timelineSlider, m_timelineText, m_volumeSlider, m_playButton, m_pauseButton, m_replayButton);
    hide();
}

void
PreviewAudio::unloadData()
{
    m_audioPlaybackWidget.unloadAudio();
}

void
PreviewAudio::hide()
{
    m_timelineSlider->hide();
    m_timelineText->hide();
    m_playButton->hide();
    m_pauseButton->hide();
    m_replayButton->hide();
    m_speakerLabel->hide();
    m_volumeSlider->hide();
}

void
PreviewAudio::show()
{
    m_timelineSlider->show();
    m_timelineText->show();
    m_playButton->show();
    m_pauseButton->show();
    m_replayButton->show();
    m_speakerLabel->show();
    m_volumeSlider->show();
}

void
PreviewAudio::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs)
{
    m_audioPlaybackWidget.loadAudio(fileEntry, pkgs);
}

void
PreviewAudio::createUi(QWidget* parentWidget, QVBoxLayout* parentLayout)
{
    // Top spacer
    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    parentLayout->addItem(verticalSpacer);

    // Timeline slider
    m_timelineSlider = new QSlider(parentWidget);
    m_timelineSlider->setOrientation(Qt::Orientation::Horizontal);
    m_timelineSlider->setMinimum(0);
    m_timelineSlider->setMaximum(100);
    parentLayout->addWidget(m_timelineSlider);

    // Timeline text
    QHBoxLayout* horizontalLayout_2 = new QHBoxLayout();
    QSpacerItem* horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_2);
    m_timelineText = new QLabel(parentWidget);
    QFont font;
    font.setPointSize(13);
    m_timelineText->setFont(font);
    horizontalLayout_2->addWidget(m_timelineText);
    parentLayout->addLayout(horizontalLayout_2);

    // Middle spacer
    QSpacerItem* verticalSpacer_3 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
    parentLayout->addItem(verticalSpacer_3);

    // Buttons
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    m_playButton = new QPushButton(parentWidget);
    m_playButton->setMinimumSize(QSize(40, 40));
    m_playButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
    horizontalLayout->addWidget(m_playButton);

    m_pauseButton = new QPushButton(parentWidget);
    m_pauseButton->setMinimumSize(QSize(40, 40));
    m_pauseButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
    horizontalLayout->addWidget(m_pauseButton);

    m_replayButton = new QPushButton(parentWidget);
    m_replayButton->setMinimumSize(QSize(40, 40));
    m_replayButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaylistRepeat));
    horizontalLayout->addWidget(m_replayButton);

    QSpacerItem* horizontalSpacer = new QSpacerItem(40, 50, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);

    m_speakerLabel = new QLabel(parentWidget);
    QPixmap pixmap = QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeHigh).pixmap(20, 20);
    m_speakerLabel->setPixmap(pixmap);
    horizontalLayout->addWidget(m_speakerLabel);

    // Volume slider
    m_volumeSlider = new QSlider(parentWidget);
    m_volumeSlider->setOrientation(Qt::Orientation::Horizontal);
    m_volumeSlider->setMaximumWidth(150);
    m_volumeSlider->setMinimum(0);
    m_volumeSlider->setMaximum(100);
    horizontalLayout->addWidget(m_volumeSlider);

    parentLayout->addLayout(horizontalLayout);

    // Bottom spacer
    QSpacerItem* verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    parentLayout->addItem(verticalSpacer_2);
}