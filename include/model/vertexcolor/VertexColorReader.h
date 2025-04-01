#pragma once

#include "BinaryReaderBuffered.h"
#include "model/vertexcolor/VertexColorStructs.h"
#include "EnumMap.h"
#include "glm/vec4.hpp"

namespace WarframeExporter::Model::VertexColor
{
	enum VertexColorType
	{
		VERTEX_COLOR_110 = 110
	};

	class VertexColorReader : public EnumMapValue
	{
	protected:
		VertexColorReader() = default;

	public:
		virtual void readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader) = 0;
		virtual void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, VertexColorHeader& outHeader) = 0;
		virtual void readBody(BinaryReader::BinaryReaderBuffered* bodyReader, std::vector<glm::u8vec4>& outData) = 0;
	};
}