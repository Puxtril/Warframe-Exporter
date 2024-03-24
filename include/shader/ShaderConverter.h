#pragma once

#include "shader/ShaderStructs.h"

namespace WarframeExporter::Shader
{
	class ShaderConverter
	{
	protected:
		ShaderConverter() = default;

    public:
        ShaderConverter(const ShaderConverter&) = delete;
		ShaderConverter operator=(const ShaderConverter&) = delete;
		
		static ShaderConverter* getInstance();
		
		virtual bool decompileShader(ShaderEntry& shaderEntry);
	};
};