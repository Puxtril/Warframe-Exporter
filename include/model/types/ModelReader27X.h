#pragma once

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model/ModelReader.h"
#include "ExporterExceptions.h"

#include "model/types/ModelReader269.h"

#include <cassert>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace WarframeExporter::Model
{
	class ModelReader27X : public ModelReader
	{
		ModelReader27X() = default;

	public:
		inline static ModelReader27X* getInstance()
		{
			static ModelReader27X* instance = new ModelReader27X();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
				(int)ModelType::MODEL_RIGGED_272,
				(int)ModelType::MODEL_RIGGED_275,
				(int)ModelType::MODEL_RIGGED_277,
				(int)ModelType::MODEL_RIGGED_278
			};
			return extTypes;
		}

		inline ScaleType ensmalleningScale() const override
		{
			return ScaleType::XYZ;
		}

		void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) override;
		void readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody) override;
	};
}
