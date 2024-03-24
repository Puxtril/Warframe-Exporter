#include "shader/ShaderConverter.h"

using namespace WarframeExporter::Shader;

ShaderConverter*
ShaderConverter::getInstance()
{
	static ShaderConverter instance;
	return &instance;
}

bool
ShaderConverter::decompileShader(ShaderEntry& shaderEntry)
{
	shaderEntry.decompiled = "";
	return true;
}