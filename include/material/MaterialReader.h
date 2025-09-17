#pragma once

#include "BinaryReaderBuffered.h"
#include "EnumMap.h"
#include "CommonHeader.h"
#include "material/MaterialStructs.h"

namespace WarframeExporter::Material
{
    class MaterialReader : public EnumMapValue
    {
    public:
        virtual MaterialExternal readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader) = 0;
        
    protected:
        std::vector<std::string> getExtraNames(BinaryReader::BinaryReaderBuffered* headerReader);
        std::vector<std::string> getHlm3Textures(BinaryReader::BinaryReaderBuffered* headerReader);
        std::vector<std::string> getShaderSet1(BinaryReader::BinaryReaderBuffered* headerReader, int count);
        std::vector<std::string> getShaderSet2(BinaryReader::BinaryReaderBuffered* headerReader, int count);
    };
};