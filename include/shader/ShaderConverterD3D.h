#pragma once

#ifdef WIN32
	#include "d3dcompiler.h"
#endif

#include "shader/ShaderConverter.h"
#include "shader/ShaderStructs.h"

namespace WarframeExporter::Shader
{
	class ShaderConverterD3D : public ShaderConverter
	{
	protected:
		ShaderConverterD3D() = default;

    public:
        ShaderConverterD3D(const ShaderConverterD3D&) = delete;
		ShaderConverterD3D operator=(const ShaderConverterD3D&) = delete;
		
		static ShaderConverterD3D* getInstance();
		
		virtual bool decompileShader(ShaderEntry& shaderEntry) override;
	};
};