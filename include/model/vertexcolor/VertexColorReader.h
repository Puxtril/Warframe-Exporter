#pragma once

#include "BinaryReaderBuffered.h"
#include "model/vertexcolor/VertexColorStructs.h"
#include "EnumMapValue.h"
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
		virtual void readHeaderDebug(BinaryReaderBuffered* headerReader) = 0;
		virtual void readHeader(BinaryReaderBuffered* headerReader, VertexColorHeader& outHeader) = 0;
		virtual void readBody(BinaryReaderBuffered* bodyReader, std::vector<glm::u8vec4>& outData) = 0;
	};
}