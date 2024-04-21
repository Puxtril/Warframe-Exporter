#include "material/types/MaterialReaderHlm3.h"

using namespace WarframeExporter::Material;

MaterialExternal
MaterialReaderHlm3::readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
    MaterialExternal extMaterial;

    extMaterial.attributes = splitAttributes(commonHeader.attributes);
    extMaterial.hlm3Textures = getHlm3Textures(headerReader);
    extMaterial.keywords = getExtraNames(headerReader);
    extMaterial.shaderSet1 = getShaderSet1(headerReader, 15);
    extMaterial.shaderSet2 = getShaderSet2(headerReader, 30);

    return extMaterial;
}