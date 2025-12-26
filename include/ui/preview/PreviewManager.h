#pragma once

#include <QtWidgets/QVBoxLayout>

#include <map>

#include "EnumMapExtractor.h"
#include "Extractor.h"

#include "ui/preview/Preview.h"
#include "ui/preview/PreviewMessage.h"
#include "ui/preview/PreviewMaterial.h"
#include "ui/preview/PreviewTexture.h"
#include "ui/preview/PreviewModel.h"
#include "ui/preview/PreviewAudio.h"

class PreviewManager
{
    LotusLib::PackagesReader* m_pkgs;
    std::map<WarframeExporter::ExtractorType, Preview*> m_previewWidgets;

public:
    PreviewManager();

    void setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonArea, QHBoxLayout* previewButtonLayout);
    void setData(LotusLib::PackagesReader* pkgs);

    void swapToFilePreview(LotusLib::FileEntry& fileEntry);
    void clearPreview();
    void playPauseAudio();

private:
    Preview* getPreview(LotusLib::FileEntry& fileEntry);
};