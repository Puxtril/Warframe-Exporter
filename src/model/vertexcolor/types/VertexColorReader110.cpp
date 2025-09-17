#include "model/vertexcolor/types/VertexColorReader110.h"

using namespace WarframeExporter::Model::VertexColor;

void
VertexColorReader110::readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader)
{
	const static std::string colorCountMsg = "Vertex color count";
	headerReader->readUInt32(0, 50000, colorCountMsg);

	const static std::string pathLenMsg = "Vertex model path length";
	headerReader->readUInt32(0, 1000, pathLenMsg);
}

void
VertexColorReader110::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, VertexColorHeader& outHeader)
{
	outHeader.colorCount = headerReader->readUInt32();

	int pathLen = headerReader->readUInt32();
	outHeader.modelPath = headerReader->readAsciiString(pathLen);
}

void
VertexColorReader110::readBody(BinaryReader::BinaryReaderBuffered* bodyReader, std::vector<glm::u8vec4>& outData)
{
	size_t colorCount = bodyReader->getLength() / 4;

	outData.resize(colorCount);
	for (size_t x = 0; x < colorCount; x++)
	{
		bodyReader->readUInt8Array(&outData[x][0], 4);
	}
}