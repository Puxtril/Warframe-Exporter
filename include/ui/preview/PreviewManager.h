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
    LotusLib::PackageCollection* m_pkgs;
    LotusLib::PackagesBin* m_pkgsBin;
    std::map<WarframeExporter::ExtractorType, Preview*> m_previewWidgets;

public:
    PreviewManager();

    void setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout, QWidget* previewButtonArea, QHBoxLayout* previewButtonLayout);
    void setData(LotusLib::PackageCollection* pkgs, LotusLib::PackagesBin* pkgsBin);

    void swapToFilePreview(LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor);
    void clearPreview();
    void playPauseAudio();

private:
    Preview* getPreview(LotusLib::FileEntry& fileEntry, WarframeExporter::Extractor* extractor);
};