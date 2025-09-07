#pragma once

#include "ui/preview/Preview.h"
#include "audio/AudioExtractorProxy.h"
#include "audio/AudioPCM/AudioPCMExtractor.h"
#include "ExporterExceptions.h"
#include "membuf.hpp"

#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QAudioOutput>
#include <QtCore/QBuffer>
#include <QtCore/QByteArray>
#include <memory>

class PreviewAudio : public Preview
{
    PreviewAudio() = default;
    PreviewAudio(const PreviewAudio&) = delete;
    const PreviewAudio& operator=(const PreviewAudio&) = delete;

    std::unique_ptr<QByteArray> m_audioData;
    std::unique_ptr<QBuffer> m_audioDataBuffer;
    QMediaPlayer m_mediaPlayer;
    QAudioOutput m_audioOutput;

public:
    static PreviewAudio* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonsArea, QHBoxLayout* layout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs) override;
};