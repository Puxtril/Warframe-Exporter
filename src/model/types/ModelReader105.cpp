#include "model/types/ModelReader105.h"

using namespace WarframeExporter::Model;

void
ModelReader105::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    headerReader->seek(0x4E, std::ios_base::cur);

    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32(0, 0, "Non-zero Morphs");
    outHeader.boneCount = headerReader->readUInt32(0, 0, "Bones on static mesh");

    headerReader->seek(0x18, std::ios_base::cur);

    skipUnk64Array(headerReader);

    headerReader->seek(0x51, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    skipUnk16Array(headerReader);

    if (this->isDCM(headerReader))
    {
        headerReader->seek(0x1, std::ios::cur);

        int dcmSubEnum = headerReader->readUInt32();
        if (dcmSubEnum == 2)
            headerReader->seek(6, std::ios::cur);
    }

    skipPhysicsStruct(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);
}

void
ModelReader105::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody)
{
    ModelReader102::getInstance()->readBody(extHeader, bodyReader, outBody);
}

bool
ModelReader105::isDCM(BinaryReader::BinaryReaderBuffered* headerReader)
{
    int check1 = headerReader->readUInt8();
    int check2 = headerReader->readUInt32();
    headerReader->seek(-5, std::ios::cur);

    if (check1 == 1 && check2 < 10)
        return true;
    return false;
}
