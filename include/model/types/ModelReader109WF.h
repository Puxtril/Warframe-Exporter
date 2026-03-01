#pragma once

#include "BinaryReader/Exceptions.h"
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
	class ModelReader109WF : public ModelReader
	{
		ModelReader109WF() = default;

	public:
		inline static ModelReader109WF* getInstance()
		{
			static ModelReader109WF* instance = new ModelReader109WF();
			return instance;
		}

		inline std::vector<std::tuple<LotusLib::Game, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, (int)ModelType::MODEL_LEVEL2_109 },
			};
			return extTypes;
		}

		inline ScaleType ensmalleningScale() const override
		{
			return ScaleType::NONE;
		}

		void readHeader(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) override;
		void readBody(const ModelHeaderExternal& extHeader, BinaryReader::Buffered* bodyReaderB, BinaryReader::Buffered* bodyReaderF, ModelBodyExternal& outBody) override;
	
	private:
		// This is 100% a workaround
		bool isMorePhysX(BinaryReader::Buffered* bodyReader);
};

	
}
