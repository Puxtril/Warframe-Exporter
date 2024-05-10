#include "shader/ShaderExtractor.h"

using namespace WarframeExporter::Shader;

ShaderExtractor::ShaderExtractor()
    : Extractor(), m_hasWarnedCompileNonWindows(false)
{
#ifdef WIN32
    m_shaderExportType = SHADER_EXPORT_D3DDECOMPILE;
#else
    m_shaderExportType = SHADER_EXPORT_BINARY;
#endif
}

ShaderExtractor*
ShaderExtractor::getInstance()
{
    static ShaderExtractor instance;
    return &instance;
}

ShaderHeaderExternal
ShaderExtractor::getHeader(LotusLib::FileEntry& fileEntry)
{
    ShaderReader* shaderReader = g_enumMapShader[fileEntry.commonHeader.type];
    return shaderReader->readHeader(&fileEntry.headerData);
}

ShaderEntry
ShaderExtractor::readEntry(ShaderReader* shaderReader, BinaryReader::BinaryReaderBuffered* bReader, const ShaderHeaderExternal& header, int index)
{
    return shaderReader->readShader(bReader, header, index);
}

std::vector<ShaderEntry>
ShaderExtractor::readAllEntries(ShaderReader* shaderReader, BinaryReader::BinaryReaderBuffered* bReader, const ShaderHeaderExternal& header)
{
    return shaderReader->readAllShaders(bReader, header);
}

void
ShaderExtractor::decompileShader(ShaderEntry& shaderEntry)
{
	_decompileShader(shaderEntry);
}

void
ShaderExtractor::decompileShaders(std::vector<ShaderEntry>& shaderEntries)
{
	for (int i = 0; i < shaderEntries.size(); i++)
	{
		_decompileShader(shaderEntries[i], i);
	}
}

void
ShaderExtractor::writeShader(const ShaderEntry& shader, const std::filesystem::path& outputDir, int shaderIndex)
{
    std::filesystem::path outputFile = outputDir / std::to_string(shaderIndex);
	
    std::ofstream out;
	if (shader.decompiled == "")
	{
		outputFile.replace_extension("bin");
		out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write(shader.bytecode.data(), shader.bytecode.size());
	}
	else
	{
		outputFile.replace_extension("hlsl");
		out.open(outputFile, std::ios::out | std::ofstream::trunc);
		out.write(shader.decompiled.c_str(), shader.decompiled.length());
	}
	out.close();
}

void
ShaderExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputDir)
{
    ShaderHeaderExternal externalHeader = getHeader(fileEntry);
    
    LotusLib::FileEntry fullFileEntry;
    try
    {
        fullFileEntry = pkgs.getPackage("ShaderPermutationDx11").getFile(fileEntry.metadata->getFullPath());
    }
    catch (std::exception&)
    {
        fullFileEntry = pkgs.getPackage("ShaderDx11").getFile(fileEntry.metadata->getFullPath());
    }
    
    ShaderReader* shaderReader = g_enumMapShader[fullFileEntry.commonHeader.type];

    std::vector<ShaderEntry> bodyEntries = readAllEntries(shaderReader, &fullFileEntry.bData, externalHeader);

    for (int iShader = 0; iShader < bodyEntries.size(); iShader++)
    {
        if (m_shaderExportType == SHADER_EXPORT_D3DDECOMPILE)
		    decompileShader(bodyEntries[iShader]);
        writeShader(bodyEntries[iShader], outputDir, iShader);
    }
}

void
ShaderExtractor::_decompileShader(ShaderEntry& shaderEntry, int index)
{
#ifdef WIN32
	ShaderConverterD3D* decompiler = ShaderConverterD3D::getInstance();
#else
	if (!m_hasWarnedCompileNonWindows)
	{
		m_logger.warn("Unable to decompile shaders on non-Windows platform");
		m_hasWarnedCompileNonWindows = true;
	}
	ShaderConverter* decompiler = ShaderConverter::getInstance();
#endif
	
	bool success = decompiler->decompileShader(shaderEntry);
	
	if (!success)
	{
		if (index == -1)
			m_logger.error("Shader decompilation failed");
		else
			m_logger.error("Shader decompilation failed (Index " + std::to_string(index) + ")");
	}
}