#pragma once

#include <map>

#include "model/ModelExporterGltf.h"
#include "level/LevelStructs.h"

using namespace fx::gltf;

namespace WarframeExporter::Level
{
	class LevelExporterGltf : public WarframeExporter::Model::ModelExporterGltf
	{
	public:
		void addModelData(const WarframeExporter::Model::ModelHeaderInternal& header, const WarframeExporter::Model::ModelBodyInternal& body, const LevelObjectInternal& levelObj);

	private:
		void addLevelInformation(const LevelObjectInternal& levelObj, std::vector<int32_t> meshIndices);
		std::vector<int32_t> getMeshNodeIndices(int32_t startIndex, int32_t endIndex);
	};
}
