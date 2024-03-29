#pragma once

#include "model/vertexcolor/VertexColorStructs.h"
#include "model/vertexcolor/VertexColorReader.h"
#include "BinaryReaderBuffered.h"
#include "ExporterExceptions.h"

#include <string>
#include <cstdint>

namespace WarframeExporter::Model::VertexColor
{
	class VertexColorReader110 : public VertexColorReader
	{
	protected:
		VertexColorReader110() = default;
		
	public:
		inline static VertexColorReader110* getInstance()
		{
			static VertexColorReader110* instance = new VertexColorReader110();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)VertexColorType::VERTEX_COLOR_110 };
			return extTypes;
		}
	
		void readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader) override;
		void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, VertexColorHeader& outHeader) override;
		void readBody(BinaryReader::BinaryReaderBuffered* bodyReader, std::vector<glm::u8vec4>& outData) override;
	};
};
