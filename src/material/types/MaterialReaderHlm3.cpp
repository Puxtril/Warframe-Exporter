#include "material/types/MaterialReaderHlm3.h"

using namespace WarframeExporter::Material;

MaterialExternal
MaterialReaderHlm3::readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
    MaterialExternal extMaterial;

    extMaterial.attributes = splitAttributes(commonHeader.attributes);
    extMaterial.hlm3Textures = getHlm3Textures(headerReader);
    extMaterial.keywords = getExtraNames(headerReader);
    extMaterial.shaderSet1 = getShaderSet1(headerReader, 16);
    headerReader->seek(3, std::ios::cur);
    extMaterial.shaderSet2 = getShaderSet2(headerReader, 29);

    return extMaterial;
}