#include "landscape/LandscapeReader.h"

using namespace WarframeExporter::Landscape;

LandscapeBodyChunkExternal
LandscapeReader::readPhysxHeightField(BinaryReader::BinaryReaderBuffered* reader)
{
    Physx::PhysxReader physxReader(reader);

    Physx::PhysxHeader physxHeader = Physx::readHeader(physxReader);

    LandscapeBodyChunkExternal body;
    body.header = Physx::HeightFieldReader::readHeader(physxReader, physxHeader);
    body.samples = Physx::HeightFieldReader::readSamples(physxReader, physxHeader, body.header);

    return body;
}