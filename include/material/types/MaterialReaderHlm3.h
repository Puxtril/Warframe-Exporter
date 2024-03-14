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
                (int)MaterialType::MATERIAL_214,
                (int)MaterialType::MATERIAL_216
            };
			return extTypes;
        }

        MaterialExternal readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader) override;
    };
};