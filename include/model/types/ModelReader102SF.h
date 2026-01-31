#pragma once

#include "BinaryReader/Exceptions.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model/ModelReader.h"
#include "ExporterExceptions.h"
#include "BinaryReader/Exceptions.h"

#include <cassert>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace WarframeExporter::Model
{
	class ModelReader102SF : public ModelReader
	{
		ModelReader102SF() = default;

	public:
		inline static ModelReader102SF* getInstance()
		{
			static ModelReader102SF* instance = new ModelReader102SF();
			return instance;
		}

		inline std::vector<std::tuple<LotusLib::Game, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, int>> extTypes = {
				{ LotusLib::Game::SOULFRAME, (int)ModelType::MODEL_STATIC_102 },
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
}
