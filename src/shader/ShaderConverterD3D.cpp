#include "shader/ShaderConverterD3D.h"

using namespace WarframeExporter::Shader;

ShaderConverterD3D*
ShaderConverterD3D::getInstance()
{
	static ShaderConverterD3D instance;
	return &instance;
}

bool
ShaderConverterD3D::decompileShader(ShaderEntry& shaderEntry)
{
#ifdef WIN32
	ID3DBlob* disassembly;

	HRESULT result = D3DDisassemble(
		(LPCVOID)shaderEntry.bytecode.data(),
		shaderEntry.bytecode.size(),
		D3D_DISASM_ENABLE_DEFAULT_VALUE_PRINTS,
		NULL,
		&disassembly
	);

	if (FAILED(result))
	{
		shaderEntry.decompiled = "";
		return false;
	}
	
	shaderEntry.decompiled = std::string(static_cast<char*>(disassembly->GetBufferPointer()));
	return true;
	
#else
	static_assert(false, "ShaderConverterD3D.decompilerShader() called on non-Windows platform");
#endif

}