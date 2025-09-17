#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace WarframeExporter::Shader
{
    enum class ShaderStage
    {
        VERTEX,
        PIXEL,
        COMPUTE
    };

    struct ShaderHeaderExternal
    {
        uint32_t shaderCount;
        uint32_t shaderCodeTotalSize;
        std::vector<uint32_t> shaderLengths;
    };

    struct ShaderEntry
    {
        std::vector<char> bytecode;
		std::string decompiled;
        uint32_t stage;
    };
};