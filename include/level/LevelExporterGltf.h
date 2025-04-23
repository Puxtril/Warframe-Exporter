#pragma once

#include <map>

#include "model/ModelExporterGltf.h"
#include "level/LevelStructs.h"

using namespace fx::gltf;

namespace WarframeExporter::Level::LevelExporterGltf
{
	std::vector<Mesh> addModel(Document& gltfDoc, const WarframeExporter::Model::ModelHeaderInternal& header, const WarframeExporter::Model::ModelBodyInternal& bodyInt, const WarframeExporter::Model::ModelBodyExternal& bodyExt, const LevelObjectInternal& levelObj);
	void addModelInstance(Document& gltfDoc, const LevelObjectInternal& levelObj, const std::vector<Mesh>& meshes);

	void _applyTransforms();
	std::vector<int32_t> _getMeshNodeIndices(Document& gltfDoc, int32_t startIndex, int32_t endIndex);
}
