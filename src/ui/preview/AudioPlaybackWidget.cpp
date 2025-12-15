#include "ui/preview/AudioPlaybackWidget.h"

void
AudioPlaybackWidget::setup()
{
    m_mediaPlayer.setAudioOutput(&m_audioOutput);
}

void
AudioPlaybackWidget::connectToWidgets(QSlider* timelineSlider, QLabel* timelineText, QSlider* volumeSlider, QPushButton* playButton, QPushButton* pauseButton, QPushButton* replayButton)
{
    m_volumeSlider = volumeSlider;
    m_timelineSlider = timelineSlider;
    m_timelineText = timelineText;
    
    int savedVolume = UiSettings::getInstance().getAudioVolume(50);
    m_volumeSlider->setValue(savedVolume);
    setVolume(savedVolume);

    connect(playButton, &QPushButton::clicked, &m_mediaPlayer, &QMediaPlayer::play);
    connect(pauseButton, &QPushButton::clicked, &m_mediaPlayer, &QMediaPlayer::pause);
    connect(replayButton, &QPushButton::clicked, this, &AudioPlaybackWidget::buttonClickedReplay);
    connect(volumeSlider, &QSlider::valueChanged, this, &AudioPlaybackWidget::setVolume);
    connect(volumeSlider, &QSlider::sliderReleased, this, &AudioPlaybackWidget::volumeSliderReleased);
    connect(&m_mediaPlayer, &QMediaPlayer::positionChanged, this, &AudioPlaybackWidget::playbackPositionChanged);
    connect(m_timelineSlider, &QSlider::sliderPressed, this, &AudioPlaybackWidget::timelineSliderPressed);
    connect(m_timelineSlider, &QSlider::sliderReleased, this, &AudioPlaybackWidget::timelineSliderReleased);
    connect(m_timelineSlider, &QSlider::sliderMoved, this, &AudioPlaybackWidget::timelinePositionDragged);
    connect(&m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &AudioPlaybackWidget::mediaStateChanged);
}

void
AudioPlaybackWidget::unloadAudio()
{
    if (m_mediaPlayer.isPlaying())
        m_mediaPlayer.stop();
    m_audioData = nullptr;
    m_audioDataBuffer = nullptr;
}

void
AudioPlaybackWidget::loadAudio(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs)
{
    membuf<> memoryBuffer;
    std::ostream extractedFile(&memoryBuffer);
    // TODO: Add this `extract` signature to base extractor class as abstract method
    switch (WarframeExporter::Audio::AudioExtractorProxy::getInstance()->peekCompressionFormat(&fileEntry.headerData))
    {
        case (WarframeExporter::Audio::AudioCompression::ADPCM):
        case (WarframeExporter::Audio::AudioCompression::PCM):
            WarframeExporter::Audio::AudioPCMExtractor::getInstance()->extract(fileEntry, pkgs, extractedFile, {});
            break;
        case (WarframeExporter::Audio::AudioCompression::OPUS):
            WarframeExporter::Audio::AudioOpusExtractor::getInstance()->extract(fileEntry, pkgs, extractedFile, {});
    }

    m_audioData = std::make_unique<QByteArray>((char*)memoryBuffer.get_memptr(), memoryBuffer.get_size());
    m_audioDataBuffer = std::make_unique<QBuffer>(m_audioData.get());
    
    m_mediaPlayer.setSourceDevice(m_audioDataBuffer.get());
}

void
AudioPlaybackWidget::mediaStateChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::MediaStatus::LoadedMedia)
    {
        playbackPositionChanged(0);
    }
}

void
AudioPlaybackWidget::playbackPositionChanged(qint64 milliseconds)
{
    if (!m_timelineSlider->isSliderDown())
    {
        qint64 percent = (double)milliseconds / (double)m_mediaPlayer.duration() * 100.0;
        m_timelineSlider->setSliderPosition(percent);
        m_timelineSlider->update();
    }

    int curSeconds = milliseconds / 1000;
    int totalSeconds = m_mediaPlayer.duration() / 1000;
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << (int)(curSeconds / 60) << ":" << std::setw(2) << (int)(curSeconds % 60)
        << " / " << std::setw(2) << (int)(totalSeconds / 60) << ":" << std::setw(2) << (int)(totalSeconds % 60);

    std::string outStr = ss.str();
    m_timelineText->setText(QString(outStr.c_str()));
    m_timelineText->update();
}

void
AudioPlaybackWidget::buttonClickedReplay()
{
    m_mediaPlayer.setPosition(0);
}

void
AudioPlaybackWidget::timelineSliderPressed()
{
    if (m_mediaPlayer.isPlaying())
    {
        m_mediaWasPlayingBeforeSliderDragEvent = true;
        m_mediaPlayer.pause();
    }
    else
    {
        m_mediaWasPlayingBeforeSliderDragEvent = false;
    }
}

void
AudioPlaybackWidget::timelineSliderReleased()
{
    if (m_mediaWasPlayingBeforeSliderDragEvent)
        m_mediaPlayer.play();
}

void
AudioPlaybackWidget::volumeSliderReleased()
{
    UiSettings::getInstance().setAudioVolume(m_volumeSlider->value());
}

void
AudioPlaybackWidget::timelinePositionDragged(int value)
{
    qint64 ms = (value / 100.0) * m_mediaPlayer.duration();
    m_mediaPlayer.setPosition(ms);
}

void
AudioPlaybackWidget::setVolume(int value)
{
    m_audioOutput.setVolume(std::pow((value / 100.0), 3));
}

bool
AudioPlaybackWidget::isPlaying()
{
    return m_mediaPlayer.isPlaying();
}
