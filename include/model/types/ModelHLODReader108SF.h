#pragma once

#include "BinaryReader/Exceptions.h"
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
	class ModelHLODReader108SF : public ModelReader
	{
		ModelHLODReader108SF() = default;

	public:
		inline static ModelHLODReader108SF* getInstance()
		{
			static ModelHLODReader108SF* instance = new ModelHLODReader108SF();
			return instance;
		}

		inline std::vector<std::tuple<LotusLib::Game, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, int>> extTypes = {
				{ LotusLib::Game::SOULFRAME, (int)ModelHLODType::MODEL_HLOD_108 },
			};
			return extTypes;
		}

		inline ScaleType ensmalleningScale() const override
		{
			return ScaleType::XYZ;
		}

		void readHeader(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) override;
		void readBody(const ModelHeaderExternal& extHeader, BinaryReader::Buffered* bodyReaderB, BinaryReader::Buffered* bodyReaderF, ModelBodyExternal& outBody) override;
	};
};
