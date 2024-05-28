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

        Physx::HeightFieldMeshSplit mesh = Physx::HeightFieldReader::convertToMeshSeparateMaterials(curChunk.header, curChunk.samples);
        LandscapeConverter::scaleChunks(mesh, landscapeHeader.chunks[i], curChunk);
        internal.chunks.push_back(mesh);
    }

    return internal;
}

LandscapeExporterGltf
LandscapeExtractor::convertToGltf(const LandscapeInternal& internal)
{
    LandscapeExporterGltf gltf;
    gltf.addLandscapeChunks(internal);
    return gltf;
}

void
LandscapeExtractor::write(LandscapeExporterGltf& gltf, const std::filesystem::path& outputPath)
{
    gltf.save(outputPath);
}

void
LandscapeExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
    LandscapeHeaderExternal extHeader = readHeader(&fileEntry.headerData, fileEntry.commonHeader);
    std::vector<LandscapeBodyChunkExternal> chunks = readLandscapeChunks(&fileEntry.bData, extHeader, fileEntry.commonHeader);
    LandscapeInternal intLandscape = formatLandscape(extHeader, chunks);
    LandscapeExporterGltf gltf = convertToGltf(intLandscape);
    write(gltf, outputPath);
}