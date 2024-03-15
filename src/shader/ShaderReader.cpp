#include "shader/ShaderReader.h"

using namespace WarframeExporter::Shader;

void
ShaderReader::readShaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, BinaryReader::BinaryReaderBuffered* bodyReader)
{
    ShaderHeaderExternal shaderHeader = readShaderHeaderDebug(headerReader);
    readShaderBodyDebug(shaderHeader, bodyReader);
}