#pragma once

#include <QtWidgets/QVBoxLayout>

#include <map>

#include "EnumMapExtractor.h"
#include "Extractor.h"

#include "ui/preview/Preview.h"
#include "ui/preview/PreviewLoading.h"
#include "ui/preview/PreviewUnsupported.h"
#include "ui/preview/PreviewMaterial.h"
#include "ui/preview/PreviewTexture.h"
#include "ui/preview/PreviewModel.h"

class PreviewManager
{
    LotusLib::PackagesReader* m_pkgs;
    WarframeExporter::Ensmallening m_ensmallening;
    std::map<WarframeExporter::ExtractorType, Preview*> m_previewWidgets;

public:
    PreviewManager();

    void setupUis(QWidget* parentWidget, QVBoxLayout* parentLayout);
    void setData(LotusLib::PackagesReader* pkgs, const WarframeExporter::Ensmallening& ensmalleningData);

    void swapToFilePreview(LotusLib::FileEntry& fileEntry);
    void clearPreview();

private:
    Preview* getPreview(LotusLib::FileEntry& fileEntry);
};