#include "model/types/ModelReader160.h"

using namespace WarframeExporter::Model;

void
ModelReader160::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    ModelReader159::getInstance()->readHeader(headerReader, header, outHeader);
}

void
ModelReader160::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReaderB, BinaryReader::BinaryReaderBuffered* bodyReaderF, ModelBodyExternal& outBody)
{
    ModelReader159::getInstance()->readBody(extHeader, bodyReaderB, bodyReaderF, outBody);
}
