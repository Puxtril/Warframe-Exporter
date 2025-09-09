#pragma once

#include "ui/preview/Preview.h"
#include "ui/preview/AudioPlaybackWidget.h"

#include <QtWidgets/QSlider>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

class PreviewAudio : public Preview
{
    PreviewAudio() = default;
    PreviewAudio(const PreviewAudio&) = delete;
    const PreviewAudio& operator=(const PreviewAudio&) = delete;

    QVBoxLayout* m_parentLayout;
    QVBoxLayout* m_rootLayout;

    QSlider* m_timelineSlider;
    QLabel* m_timelineText;
    QPushButton* m_playButton;
    QPushButton* m_pauseButton;
    QPushButton* m_replayButton;
    QSlider* m_volumeSlider;
    QLabel* m_speakerLabel;

    AudioPlaybackWidget m_audioPlaybackWidget;

public:
    static PreviewAudio* getInstance(); 

    void setupUi(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonsArea, QHBoxLayout* layout) override;
    void unloadData() override;
    void hide() override;
    void show() override;
    void setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs) override;

private:
    void createUi(QWidget* parentWidget);
};