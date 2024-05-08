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

namespace WarframeExporter::Model
{
	class ModelReader272 : public ModelReader
	{
		ModelReader272() = default;

	public:
		inline static ModelReader272* getInstance()
		{
			static ModelReader272* instance = new ModelReader272();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)ModelType::MODEL_RIGGED_272 };
			return extTypes;
		}

		inline ScaleType ensmalleningScale() const override
		{
			return ScaleType::XYZ;
		}

		void readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) override;
		void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) override;
		void readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader) override;
		void readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody) override;
	};
}
