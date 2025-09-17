#pragma once

#include "ui/preview/Preview.h"
#include "audio/AudioExtractorProxy.h"
#include "audio/AudioPCM/AudioPCMExtractor.h"
#include "ExporterExceptions.h"
#include "membuf.hpp"
#include "ui/Settings.h"

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QtCore/QBuffer>
#include <QtCore/QByteArray>

#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <memory>
#include <cmath>

class AudioPlaybackWidget : public QObject
{
    Q_OBJECT

    QSlider* m_volumeSlider;
    QSlider* m_timelineSlider;
    QLabel* m_timelineText;

    std::unique_ptr<QByteArray> m_audioData;
    std::unique_ptr<QBuffer> m_audioDataBuffer;
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;

    bool m_mediaWasPlayingBeforeSliderDragEvent;

public:
    void setup();
    void connectToWidgets(QSlider* timelineSlider, QLabel* timelineText, QSlider* volumeSlider, QPushButton* playButton, QPushButton* pauseButton, QPushButton* replayButton);
    void unloadAudio();
    void loadAudio(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs);

    void mediaStateChanged(QMediaPlayer::MediaStatus status);
    void playbackPositionChanged(qint64 milliseconds);
    void buttonClickedReplay();
    void timelineSliderPressed();
    void timelineSliderReleased();
    void volumeSliderReleased();
    // `value` is 0-100
    void timelinePositionDragged(int value);
    void setVolume(int value);
};