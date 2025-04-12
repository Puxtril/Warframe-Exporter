#pragma once

#include "model/ModelExporterGltf.h"
#include "levelstatic/LevelStaticStructs.h"
#include "level/LevelExporterGltf.h"

#include <iostream>

using namespace fx::gltf;

namespace WarframeExporter::LevelStatic::ExporterGltf
{
	void addModelData(
        Document& gltfDoc,
        const WarframeExporter::Model::ModelHeaderInternal& header,
        const WarframeExporter::Model::ModelBodyInternal& modelBodyInt,
        const WarframeExporter::Model::ModelBodyExternal& modelBodyExt,
        const LevelStaticHeaderExternal& levelStaticHeaderExt,
        const LevelStaticObjectExternal& levelStaticObj
    );

	void _addLevelInformation(Document& gltfDoc, const LevelStaticHeaderExternal& levelStaticHeaderExt, const LevelStaticObjectExternal& levelObj, std::vector<int32_t> meshIndices);
}