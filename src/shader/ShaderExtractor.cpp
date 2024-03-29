#include "shader/ShaderExtractor.h"

using namespace WarframeExporter::Shader;

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
ShaderExtractor::writeShader(const ShaderEntry& shader, const std::filesystem::path& outputDir, int shaderIndex)
{
    std::filesystem::path outputFile = outputDir / std::to_string(shaderIndex);
    outputFile.replace_extension("bin");

    std::ofstream out;
	out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
	out.write(shader.bytecode.data(), shader.bytecode.size());
	out.close();
}

void
ShaderExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputDir)
{
    ShaderHeaderExternal externalHeader = getHeader(fileEntry);
    
    LotusLib::FileEntry fullFileEntry;
    try
    {
        fullFileEntry = pkgs.getPackage("ShaderPermutationDx11").getFile(fileEntry.metadata.getFullPath());
    }
    catch (std::exception&)
    {
        fullFileEntry = pkgs.getPackage("ShaderDx11").getFile(fileEntry.metadata.getFullPath());
    }
    
    ShaderReader* shaderReader = g_enumMapShader[fullFileEntry.commonHeader.type];

    std::vector<ShaderEntry> bodyEntries = readAllEntries(shaderReader, &fullFileEntry.bData, externalHeader);

    for (size_t iShader = 0; iShader < bodyEntries.size(); iShader++)
    {
        writeShader(bodyEntries[iShader], outputDir, iShader);
    }
}

void
ShaderExtractor::extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData)
{
    LotusLib::FileEntry fullFileEntry;
    try
    {
        fullFileEntry = pkgs.getPackage("ShaderPermutationDx11").getFile(fileEntry.metadata.getFullPath());
    }
    catch (std::exception&)
    {
        fullFileEntry = pkgs.getPackage("ShaderDx11").getFile(fileEntry.metadata.getFullPath());
    }
    
    ShaderReader* shaderReader = g_enumMapShader[fullFileEntry.commonHeader.type];

    shaderReader->readShaderDebug(&fullFileEntry.headerData, &fullFileEntry.bData);
}