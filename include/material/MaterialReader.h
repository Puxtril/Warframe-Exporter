#pragma once

#include "BinaryReaderBuffered.h"
#include "EnumMapValue.h"
#include "CommonHeader.h"
#include "material/MaterialStructs.h"

namespace WarframeExporter::Material
{
    class MaterialReader : public EnumMapValue
    {
    public:
        virtual MaterialExternal readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader) = 0;
        
    protected:
        std::vector<std::string> splitAttributes(const std::string& attributes);
        std::vector<std::string> getExtraNames(BinaryReader::BinaryReaderBuffered* headerReader);
        std::vector<std::string> getHlm3Textures(BinaryReader::BinaryReaderBuffered* headerReader);
    };
};