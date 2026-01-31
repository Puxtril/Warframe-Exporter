#pragma once

#include "BinaryReader/Buffered.h"
#include "EnumMap.h"
#include "LotusLib/CommonHeader.h"
#include "material/MaterialStructs.h"

namespace WarframeExporter::Material
{
    class MaterialReader : public EnumMapValue
    {
    public:
        virtual MaterialExternal readData(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& commonHeader) = 0;
        
    protected:
        std::vector<std::string> getExtraNames(BinaryReader::Buffered* headerReader);
        std::vector<std::string> getHlm3Textures(BinaryReader::Buffered* headerReader);
        std::vector<std::string> getShaderSet1(BinaryReader::Buffered* headerReader, int count);
        std::vector<std::string> getShaderSet2(BinaryReader::Buffered* headerReader, int count);
    };
};