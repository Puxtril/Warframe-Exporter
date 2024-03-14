#include "material/types/MaterialReaderBasic.h"

using namespace WarframeExporter::Material;

MaterialExternal
MaterialReaderBasic::readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
    MaterialExternal extMaterial;

    extMaterial.attributes = splitAttributes(commonHeader.attributes);
    extMaterial.keywords = getExtraNames(headerReader);

    return extMaterial;
}