#include "material/types/MaterialReaderHlm3.h"

using namespace WarframeExporter::Material;

MaterialExternal
MaterialReaderHlm3::readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
    MaterialExternal extMaterial;

    extMaterial.attributes = splitAttributes(commonHeader.attributes);
    extMaterial.hlm3Textures = getHlm3Textures(headerReader);
    extMaterial.keywords = getExtraNames(headerReader);

    return extMaterial;
}