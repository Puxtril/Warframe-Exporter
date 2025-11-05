#include "model/types/ModelReader300.h"

using namespace WarframeExporter::Model;

void
ModelReader300::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader)
{
    headerReader->seek(0x30, std::ios_base::cur);

    skipPhysicsStruct(headerReader);

    headerReader->seek(0x2C, std::ios_base::cur);
    headerReader->readSingleArray(&outHeader.ensmallening1[0], 4);
    headerReader->readSingleArray(&outHeader.ensmallening2[0], 4);

    readWeightedBones(headerReader, outHeader.weightedBoneNames);

    // Main vertex counts
    outHeader.faceCount = headerReader->readUInt32();
    outHeader.boneCount = headerReader->readUInt32();
    outHeader.vertexCount = headerReader->readUInt32();
    outHeader.morphCount = headerReader->readUInt32(0, 0, "Non-zero Morphs");

    headerReader->seek(0x8, std::ios_base::cur);
    
    skipUnk64Array(headerReader);
    
    headerReader->seek(0x31, std::ios_base::cur);

    readBoneTree(headerReader, outHeader.boneTree);

    uint32_t unkStructCount = skipUnknownStructs(headerReader);

    headerReader->seek(0x1A, std::ios_base::cur);
    outHeader.bodySkipLen1 = headerReader->readUInt32(0, 1000, "BodySkipLen1");
    headerReader->seek(0x10 * unkStructCount, std::ios_base::cur);
    headerReader->seek(0x8, std::ios_base::cur);

    readMeshInfos(headerReader, outHeader.meshInfos);

    skipUnk16Array(headerReader);

    skipMorphs(headerReader);

    readBoneMaps(headerReader, outHeader.boneMaps);

    outHeader.bodySkipLen2 = skipMorphStructsAndFindSkip(headerReader);

    headerReader->seek(0x2F, std::ios_base::cur);

    // Sub models?
    uint32_t subModelCount = headerReader->readUInt32(0, 30, "Submodel count");
    for (uint32_t x = 0; x < subModelCount; x++)
    {
        headerReader->seek(0xC, std::ios::cur);
        uint32_t nameCount = headerReader->readUInt32(0, 100, "Sub-model name");
        headerReader->seek(nameCount, std::ios::cur);
        headerReader->seek(80, std::ios::cur);
    }

    headerReader->seek(0x24, std::ios::cur);

    skipPhysicsStruct(headerReader);

    readPhysxMeshes(headerReader, outHeader.physXMeshes);

    readErrors(headerReader, outHeader.errorMsgs);

    if (headerReader->tell() != headerReader->getLength())
        throw unknown_format_error("Did not reach end of file");
}

void
ModelReader300::readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReaderB, BinaryReader::BinaryReaderBuffered* bodyReaderF, ModelBodyExternal& outBody)
{
    ModelReader290::getInstance()->readBody(extHeader, bodyReaderB, bodyReaderF, outBody);
}