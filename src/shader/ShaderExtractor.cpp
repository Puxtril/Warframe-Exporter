#include "shader/ShaderExtractor.h"

using namespace WarframeExporter::Shader;

ShaderExtractor*
ShaderExtractor::getInstance()
{
    static ShaderExtractor instance;
    return &instance;
}

void
ShaderExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{

}

void
ShaderExtractor::extractDebug(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData)
{

}