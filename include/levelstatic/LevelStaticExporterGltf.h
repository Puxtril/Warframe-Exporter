#pragma once

#include "model/ModelExporterGltf.h"
#include "levelstatic/LevelStaticStructs.h"
#include "level/LevelExporterGltf.h"

using namespace fx::gltf; 

namespace WarframeExporter::LevelStatic::ExporterGltf
{
    struct ModelInfo
    {
        Attributes attributes;
        std::vector<int32_t> indexIndices;
        std::vector<WarframeExporter::Model::MeshInfoInternal> meshInfos;
    };

	ModelInfo addModel(
        Document& gltfDoc,
        const WarframeExporter::Model::ModelHeaderInternal& header,
        const WarframeExporter::Model::ModelBodyInternal& modelBodyInt,
        const WarframeExporter::Model::ModelBodyExternal& modelBodyExt
    );

	void addModelInstance(
        Document& gltfDoc,
        const LevelStaticHeaderExternal& levelStaticHeaderExt,
        const LevelStaticObjectExternal& levelObj,
        WarframeExporter::LevelStatic::ExporterGltf::ModelInfo modelInfo
    );
}