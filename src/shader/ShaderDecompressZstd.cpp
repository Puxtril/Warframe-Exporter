#include "shader/ShaderDecompressZstd.h"

using namespace WarframeExporter::Shader;

WarframeDecompressZstd::WarframeDecompressZstd()
    : m_isInitilized(false), m_errorReading(), m_zstdDictArray{}, m_zstdContext(nullptr)
{ }

void
WarframeDecompressZstd::initilize(const LotusLib::PackageCollection& pkgsReader)
{
    if (m_isInitilized)
        return;

    m_isInitilized = true;
    m_errorReading = true;

    if (!pkgsReader.hasPackage("ShaderDictionaryDx11"))
        return;
    LotusLib::Package pkg = pkgsReader.getPackage("ShaderDictionaryDx11");

    BinaryReader::Buffered bodyReader(pkg.getFile(LotusLib::PkgSplitType::BODY, "/EE/Types/GraphicsRes/ShaderDictionary"));

    m_zstdContext = ZSTD_createDCtx();

    std::vector<size_t> zstdDictOffsets = findZstdDictionaryOffsets(bodyReader);

    for (int iDict = 0; iDict < zstdDictOffsets.size(); iDict++)
    {
        const unsigned char* start = bodyReader.getPtr().data() + zstdDictOffsets[iDict];
        const size_t nextStart = iDict+2 <= zstdDictOffsets.size() ? zstdDictOffsets[iDict+1] : bodyReader.getLength();
        const size_t length = nextStart - zstdDictOffsets[iDict];

        ZSTD_DDict* dict = ZSTD_createDDict(start, length);
        if (dict == nullptr)
        {
            WarframeExporter::Logger::getInstance().warn("Error creating zstd Dictionary " + std::to_string(iDict));
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
WarframeDecompressZstd::findZstdDictionaryOffsets(BinaryReader::Buffered& reader) const
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