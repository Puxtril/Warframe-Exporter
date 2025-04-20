#pragma once

#include <map>

#include "model/ModelExporterGltf.h"
#include "level/LevelStructs.h"

using namespace fx::gltf;

namespace WarframeExporter::Level::LevelExporterGltf
{
	struct ModelInfo
	{
		Attributes attributes;
		std::vector<int32_t> indexIndices;
		std::vector<WarframeExporter::Model::MeshInfoInternal> meshInfos;
	};

	ModelInfo addModel(Document& gltfDoc, const WarframeExporter::Model::ModelHeaderInternal& header, const WarframeExporter::Model::ModelBodyInternal& bodyInt, const WarframeExporter::Model::ModelBodyExternal& bodyExt, const LevelObjectInternal& levelObj);
	void addModelInstance(Document& gltfDoc, const LevelObjectInternal& levelObj, const ModelInfo& modelInfo);

	void _applyTransforms();
	std::vector<int32_t> _getMeshNodeIndices(Document& gltfDoc, int32_t startIndex, int32_t endIndex);
}
