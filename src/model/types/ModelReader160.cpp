#include "model/types/ModelReader160.h"

using namespace WarframeExporter::Model;

void
ModelReader160::readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header)
{
    ModelReader159::getInstance()->readHeaderDebug(headerReader, ensmalleningData, header);
}

void
ModelReader160::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    ModelReader159::getInstance()->readHeader(headerReader, ensmalleningData, header, outHeader);
}

void
ModelReader160::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
{
    ModelReader159::getInstance()->readBody(extHeader, bodyReader, outBody);
}

void
ModelReader160::readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader)
{

}