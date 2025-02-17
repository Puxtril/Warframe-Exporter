#pragma once

#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model/ModelReader.h"
#include "ExporterExceptions.h"

#include <cassert>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "model/types/ModelReader290.h"

namespace WarframeExporter::Model
{
	class ModelReader300 : public ModelReader
	{
		ModelReader300() = default;

	public:
		inline static ModelReader300* getInstance()
		{
			static ModelReader300* instance = new ModelReader300();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = {
				(int)ModelType::MODEL_PACKED_300,
				(int)ModelType::MODEL_PACKED_301
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
