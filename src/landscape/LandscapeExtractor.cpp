#include "landscape/LandscapeExtractor.h"

using namespace WarframeExporter::Landscape;

LandscapeExtractor*
LandscapeExtractor::getInstance()
{
    static LandscapeExtractor instance;
    return &instance;
}

LandscapeHeaderExternal
LandscapeExtractor::readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader)
{
    LandscapeReader* reader = g_enumMapLandscape[commonHeader.type];
    LandscapeHeaderExternal external = reader->readHeader(headerReader);
    return external;
}

std::vector<LandscapeBodyChunkExternal>
LandscapeExtractor::readLandscapeChunks(BinaryReader::BinaryReaderBuffered* bodyReader, const LandscapeHeaderExternal extHeader, const LotusLib::CommonHeader& commonHeader)
{
    LandscapeReader* reader = g_enumMapLandscape[commonHeader.type];
    std::vector<LandscapeBodyChunkExternal> chunks = reader->readBody(bodyReader, extHeader);
    return chunks;
}

LandscapeInternal
LandscapeExtractor::formatLandscape(const LandscapeHeaderExternal& landscapeHeader, const std::vector<LandscapeBodyChunkExternal>& landscapeBody)
{
    LandscapeInternal internal;

    LandscapeConverter::positionChunks(landscapeHeader, landscapeBody, internal);
    internal.materialPathArrays = std::move(landscapeHeader.materialPathArrays);

    for (size_t i = 0; i < landscapeBody.size(); i++)
    {
        const LandscapeBodyChunkExternal& curChunk = landscapeBody[i];

        Physx::HeightFieldMesh mesh = Physx::HeightFieldReader::convertToMesh(curChunk.header, curChunk.samples);
        LandscapeConverter::scaleChunks(mesh, landscapeHeader.chunks[i], curChunk);
        internal.chunks.push_back(mesh);
    }

    LandscapeConverter::addTransforms(internal);
    return internal;
}

Document
LandscapeExtractor::convertToGltf(const LandscapeInternal& internal)
{
    Document gltf;
    LandscapeExporterGltf::addLandscapeChunks(gltf, internal);
    return gltf;
}

void
LandscapeExtractor::write(Document& gltf, const std::filesystem::path& outputPath)
{
    Model::ModelExporterGltf::save(gltf, outputPath);
}

void
LandscapeExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, bool dryRun)
{
    LandscapeHeaderExternal extHeader = readHeader(&fileEntry.headerData, fileEntry.commonHeader);
    std::vector<LandscapeBodyChunkExternal> chunks = readLandscapeChunks(&fileEntry.bData, extHeader, fileEntry.commonHeader);
    LandscapeInternal intLandscape = formatLandscape(extHeader, chunks);
    Document gltf = convertToGltf(intLandscape);
    if (!dryRun)
        write(gltf, outputPath);
}
