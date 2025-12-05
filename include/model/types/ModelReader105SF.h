#pragma once

#include "BinaryReaderExceptions.h"
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

namespace WarframeExporter::Model
{
	class ModelReader105SF : public ModelReader
	{
		ModelReader105SF() = default;

	public:
		inline static ModelReader105SF* getInstance()
		{
			static ModelReader105SF* instance = new ModelReader105SF();
			return instance;
		}

		inline std::vector<std::tuple<LotusLib::Game, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, int>> extTypes = {
				{ LotusLib::Game::SOULFRAME, (int)ModelType::MODEL_LEVEL1_105 },
			};
			return extTypes;
		}

		inline ScaleType ensmalleningScale() const override
		{
			return ScaleType::NONE;
		}

		void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) override;
		void readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReaderB, BinaryReader::BinaryReaderBuffered* bodyReaderF, ModelBodyExternal& outBody) override;

	private:
		bool isMorePhysX(BinaryReader::BinaryReaderBuffered* bodyReader);
};

	
}
