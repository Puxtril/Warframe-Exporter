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
    m_parentLayout = parentLayout;
    createUi(parentWidget);
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
    m_groupBox->hide();
    m_timelineSlider->hide();
    m_timelineText->hide();
    m_playButton->hide();
    m_pauseButton->hide();
    m_replayButton->hide();
    m_speakerLabel->hide();
    m_volumeSlider->hide();
    m_parentLayout->removeItem(m_rootLayout);
}

void
PreviewAudio::show()
{
    m_groupBox->show();
    m_timelineSlider->show();
    m_timelineText->show();
    m_playButton->show();
    m_pauseButton->show();
    m_replayButton->show();
    m_speakerLabel->show();
    m_volumeSlider->show();
    m_parentLayout->addItem(m_rootLayout);
}

void
PreviewAudio::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs)
{
    m_audioPlaybackWidget.loadAudio(fileEntry, pkgs);
}

void
PreviewAudio::createUi(QWidget* parentWidget)
{
    m_rootLayout = new QVBoxLayout();

    // Top spacer
    QSpacerItem* verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    m_rootLayout->addItem(verticalSpacer);

    // Group box
    m_groupBox = new QGroupBox(parentWidget);
    m_rootLayout->addWidget(m_groupBox);
    QVBoxLayout* groupboxLayout = new QVBoxLayout(m_groupBox);

    // Timeline slider
    m_timelineSlider = new QSlider(m_groupBox);
    m_timelineSlider->setOrientation(Qt::Orientation::Horizontal);
    m_timelineSlider->setMinimum(0);
    m_timelineSlider->setMaximum(100);
    groupboxLayout->addWidget(m_timelineSlider);

    // Timeline text
    QHBoxLayout* horizontalLayout_2 = new QHBoxLayout();
    QSpacerItem* horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    horizontalLayout_2->addItem(horizontalSpacer_2);
    m_timelineText = new QLabel(m_groupBox);
    QFont font;
    font.setPointSize(13);
    m_timelineText->setFont(font);
    horizontalLayout_2->addWidget(m_timelineText);
    groupboxLayout->addLayout(horizontalLayout_2);

    // Middle spacer
    QSpacerItem* verticalSpacer_3 = new QSpacerItem(20, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
    groupboxLayout->addItem(verticalSpacer_3);

    // Buttons
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    m_playButton = new QPushButton(m_groupBox);
    m_playButton->setMinimumSize(QSize(40, 40));
    m_playButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackStart));
    horizontalLayout->addWidget(m_playButton);

    m_pauseButton = new QPushButton(m_groupBox);
    m_pauseButton->setMinimumSize(QSize(40, 40));
    m_pauseButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaybackPause));
    horizontalLayout->addWidget(m_pauseButton);

    m_replayButton = new QPushButton(m_groupBox);
    m_replayButton->setMinimumSize(QSize(40, 40));
    m_replayButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::MediaPlaylistRepeat));
    horizontalLayout->addWidget(m_replayButton);

    QSpacerItem* horizontalSpacer = new QSpacerItem(40, 50, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);
    horizontalLayout->addItem(horizontalSpacer);

    m_speakerLabel = new QLabel(m_groupBox);
    QPixmap pixmap = QIcon::fromTheme(QIcon::ThemeIcon::AudioVolumeHigh).pixmap(20, 20);
    m_speakerLabel->setPixmap(pixmap);
    horizontalLayout->addWidget(m_speakerLabel);

    // Volume slider
    m_volumeSlider = new QSlider(m_groupBox);
    m_volumeSlider->setOrientation(Qt::Orientation::Horizontal);
    m_volumeSlider->setMaximumWidth(150);
    m_volumeSlider->setMinimum(0);
    m_volumeSlider->setMaximum(100);
    horizontalLayout->addWidget(m_volumeSlider);

    groupboxLayout->addLayout(horizontalLayout);

    // Bottom spacer
    QSpacerItem* verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);
    m_rootLayout->addItem(verticalSpacer_2);
}