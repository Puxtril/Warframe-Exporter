#include "material/types/MaterialReaderBasic.h"

using namespace WarframeExporter::Material;

MaterialExternal
MaterialReaderBasic::readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
    MaterialExternal extMaterial;

    extMaterial.attributes = splitAttributes(commonHeader.attributes);
    extMaterial.keywords = getExtraNames(headerReader);
    extMaterial.shaderSet1 = getShaderSet1(headerReader, 16);
    headerReader->seek(3, std::ios::cur);
    extMaterial.shaderSet2 = getShaderSet2(headerReader, 29);

    return extMaterial;
}