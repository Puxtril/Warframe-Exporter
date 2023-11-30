#pragma once

#include "BinaryReaderExceptions.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model/ModelReader.h"
#include "ExporterExceptions.h"

#include "model/types/ModelReader159.hpp"

#include <cassert>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace WarframeExporter::Model
{
	class ModelReader160 : public ModelReader
	{
		ModelReader160() = default;

	public:
		inline static ModelReader160* getInstance()
		{
			static ModelReader160* instance = new ModelReader160();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)ModelType::MODEL_TERRAIN_160 };
			return extTypes;
		}

		inline ScaleType ensmalleningScale() const override
		{
			return ScaleType::XZ;
		}

		void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) override
		{
			ModelReader159::getInstance()->readHeaderDebug(headerReader, ensmalleningData, header);
		}

		void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) override
		{
			ModelReader159::getInstance()->readHeader(headerReader, ensmalleningData, header, outHeader);
		}

		void readBody(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody) override
		{
			ModelReader159::getInstance()->readBody(extHeader, bodyReader, outBody);
		}

		void readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader) override
		{

		}
	};
}
