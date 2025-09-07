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
    m_audioOutput.setVolume(100);
    m_mediaPlayer.setAudioOutput(&m_audioOutput);
}

void
PreviewAudio::unloadData()
{
    if (m_mediaPlayer.isPlaying())
        m_mediaPlayer.stop();
    m_audioData = nullptr;
    m_audioDataBuffer = nullptr;
}

void
PreviewAudio::hide()
{
}

void
PreviewAudio::show()
{
}

void
PreviewAudio::setupWidget(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs)
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
    m_mediaPlayer.play();
}
