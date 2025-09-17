#include "material/types/MaterialReaderBasic.h"

using namespace WarframeExporter::Material;

MaterialExternal
MaterialReaderBasic::readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
    MaterialExternal extMaterial;

    extMaterial.attributes = commonHeader.attributes;
    extMaterial.keywords = getExtraNames(headerReader);
    extMaterial.shaderSet1 = getShaderSet1(headerReader, 15);
    extMaterial.shaderSet2 = getShaderSet2(headerReader, 30);

    return extMaterial;
}