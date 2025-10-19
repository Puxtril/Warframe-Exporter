#pragma once

#include "BinaryReaderExceptions.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model/ModelReader.h"
#include "ExporterExceptions.h"
#include "model/ModelTypes.h"

#include <cassert>
#include <cstring>
#include <iomanip>
#include <string_view>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace WarframeExporter::Model
{
	class ModelHLODReader : public ModelReader
	{
		ModelHLODReader() = default;

	public:
		inline static ModelHLODReader* getInstance()
		{
			static ModelHLODReader* instance = new ModelHLODReader();
			return instance;
		}

		inline std::vector<std::tuple<LotusLib::Game, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, (int)ModelHLODType::MODEL_HLOD_102 },
				{ LotusLib::Game::WARFRAME, (int)ModelHLODType::MODEL_HLOD_105 },
				{ LotusLib::Game::SOULFRAME, (int)ModelHLODType::MODEL_HLOD_105 },
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
};
