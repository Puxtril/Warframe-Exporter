#include "landscape/types/LandscapeReader42.h"

using namespace WarframeExporter::Landscape;

LandscapeReader42*
LandscapeReader42::getInstance()
{
    static LandscapeReader42 instance;
    return &instance;
}

LandscapeHeaderExternal
LandscapeReader42::readHeader(BinaryReader::BinaryReaderBuffered* reader)
{
    LandscapeHeaderExternal outHeader;

    uint32_t pathArrAcount = reader->readUInt32();
    
    reader->seek((4 * 2), std::ios::cur);

    outHeader.rowCount = reader->readUInt32();
    outHeader.columnCount = reader->readUInt32();

    reader->seek(2, std::ios::cur);

    // 3 ints, maybe important
    reader->seek(4 * 3, std::ios::cur);

    reader->seek(0xB, std::ios::cur);

    outHeader.materialPathArrays.resize(9);
    for (size_t i = 0; i < 9; i++)
    {
        std::vector<std::string>& curPathArray = outHeader.materialPathArrays[i];

        uint32_t curSubarrayLen = reader->readUInt32();
        for (uint32_t ii = 0; ii < curSubarrayLen; ii++)
        {
            uint32_t curPathLen = reader->readUInt32();
            std::string curPath = reader->readAsciiString(curPathLen);
            curPathArray.push_back(curPath);
        }
    }

    uint32_t byteArrCount = reader->readUInt32();
    reader->seek(byteArrCount, std::ios::cur);

    reader->seek(0x14, std::ios::cur);

    uint32_t chunkCount = reader->readUInt32();
    outHeader.chunks.resize(chunkCount);
    for (size_t i = 0; i < chunkCount; i++)
    {
        LandscapeHeaderChunkExternal& curChunk = outHeader.chunks[i];

        //reader->seek(0x8C, std::ios::cur);
        reader->seek(0x3C, std::ios::cur);

        curChunk.scale = {
            reader->readFloat(),
            reader->readFloat(),
            reader->readFloat()
        };

        reader->seek(0x44, std::ios::cur);

        curChunk.bodyLen = reader->readUInt32();
        reader->seek(8, std::ios::cur);
    }

    return outHeader;
}

std::vector<LandscapeBodyChunkExternal>
LandscapeReader42::readBody(BinaryReader::BinaryReaderBuffered* reader, const LandscapeHeaderExternal& header)
{
    std::vector<LandscapeBodyChunkExternal> heightfieldChunks;

    for (size_t i = 0; i < header.chunks.size(); i++)
    {
        size_t startPos = reader->tell();
        LandscapeBodyChunkExternal chunk = readPhysxHeightField(reader);
        heightfieldChunks.push_back(std::move(chunk));
        size_t endPos = reader->tell();

        if (endPos - startPos != header.chunks[i].bodyLen)
            throw unknown_format_error("Read incorrect size for Physx chunk " + std::to_string(i) + ": Read " + std::to_string(endPos - startPos) + ", expected " + std::to_string(header.chunks[i].bodyLen));
    }

    return heightfieldChunks;
}