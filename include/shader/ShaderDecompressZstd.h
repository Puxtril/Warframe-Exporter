#pragma once

#include <vector>

#include "BinaryReaderBuffered.h"
#include "ExporterLogger.h"
#include "LotusLib.h"
#include "zstd.h"

namespace WarframeExporter::Shader
{
    class WarframeDecompressZstd
    {
        bool m_isInitilized;
        bool m_errorReading;
        std::vector<ZSTD_DDict*> m_zstdDictArray;
        ZSTD_DCtx* m_zstdContext;

    public:
        WarframeDecompressZstd();

        void initilize(LotusLib::PackagesReader& pkgsReader);
        size_t decompress(const char* inData, size_t inDataLen, const char* outData, size_t outDataCapacity, int dictIndex) const;
        bool initSuccess() const;

    private:
        std::vector<size_t> findZstdDictionaryOffsets(BinaryReader::BinaryReaderBuffered& reader) const;
    };
};