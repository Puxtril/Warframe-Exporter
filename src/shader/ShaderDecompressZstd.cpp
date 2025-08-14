#include "shader/ShaderDecompressZstd.h"

using namespace WarframeExporter::Shader;

WarframeDecompressZstd::WarframeDecompressZstd()
    : m_isInitilized(false), m_errorReading(), m_zstdDictArray{}, m_zstdContext(nullptr)
{ }

void
WarframeDecompressZstd::initilize(LotusLib::PackagesReader& pkgsReader)
{
    if (m_isInitilized)
        return;

    m_isInitilized = true;
    m_errorReading = true;

    std::optional<LotusLib::PackageReader> pkg = pkgsReader.getPackage("ShaderDictionaryDx11");
    if (!pkg)
        return;

    LotusLib::FileEntry fileEntry = pkg.value().getFile("/EE/Types/GraphicsRes/ShaderDictionary");

    m_zstdContext = ZSTD_createDCtx();

    std::vector<size_t> zstdDictOffsets = findZstdDictionaryOffsets(fileEntry.bData);

    for (int iDict = 0; iDict < zstdDictOffsets.size(); iDict++)
    {
        const unsigned char* start = fileEntry.bData.getPtr().data() + zstdDictOffsets[iDict];
        const size_t nextStart = iDict+2 <= zstdDictOffsets.size() ? zstdDictOffsets[iDict+1] : fileEntry.bData.getLength();
        const size_t length = nextStart - zstdDictOffsets[iDict];

        ZSTD_DDict* dict = ZSTD_createDDict(start, length);
        if (dict == nullptr)
        {
            std::cout << "Error creating zstd Dictionary " << iDict << std::endl;
            return;
        }

        m_zstdDictArray.push_back(dict);
    }

    // The dictionaries have magic numbers, compressed entries do not.
    ZSTD_DCtx_setParameter(m_zstdContext, ZSTD_d_experimentalParam1, 1);

    WarframeExporter::Logger::getInstance().info("Read " + std::to_string(m_zstdDictArray.size()) + " zstd dictionaries from ShaderDictionaryDx11");
    m_errorReading = false;
}

size_t
WarframeDecompressZstd::decompress(const char* inData, size_t inDataLen, const char* outData, size_t outDataCapacity, int dictIndex) const
{
    if (dictIndex >= m_zstdDictArray.size() || m_errorReading)
        return 0;

    return ZSTD_decompress_usingDDict(
        m_zstdContext,
        (void*)outData,
        outDataCapacity,
        inData,
        inDataLen,
        m_zstdDictArray[dictIndex]
    );
}

bool
WarframeDecompressZstd::initSuccess() const
{
    return !m_errorReading;
}


std::vector<size_t>
WarframeDecompressZstd::findZstdDictionaryOffsets(BinaryReader::BinaryReaderBuffered& reader) const
{
    std::vector<size_t> foundOffsets;
    reader.seek(0, std::ios::beg);

    while (reader.tell() + 4 < reader.getLength())
    {
        if (reader.readUInt32() == 0xEC30A437)
            foundOffsets.push_back(reader.tell() - 4);
        reader.seek(0x10000 - 4, std::ios::cur);
    }

    return foundOffsets;
}