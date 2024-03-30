#include "shader/ShaderReader.h"

using namespace WarframeExporter::Shader;

void
ShaderReader::readShaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, BinaryReader::BinaryReaderBuffered* bodyReader)
{
    ShaderHeaderExternal shaderHeader = readShaderHeaderDebug(headerReader);
    if (shaderHeader.shaderCount > 0)
        readShaderBodyDebug(shaderHeader, bodyReader);
}