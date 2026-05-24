#pragma once

#include "material/MaterialReader.h"
#include "material/MaterialTypes.h"

namespace WarframeExporter::Material
{
    class MaterialReaderHlm3 : public MaterialReader
    {
    public:
        inline static MaterialReaderHlm3* getInstance()
		{
			static MaterialReaderHlm3* instance = new MaterialReaderHlm3();
			return instance;
		}

        inline std::vector<int> getEnumMapKeys() const override
        {
            std::vector<int> extTypes = {
                (int)MaterialHLMType::MATERIAL_HLM_214,
                (int)MaterialHLMType::MATERIAL_HLM_216,
                (int)MaterialHLMType::MATERIAL_HLM_217
            };
			return extTypes;
        }

        MaterialExternal readData(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& commonHeader) override;
    };
};