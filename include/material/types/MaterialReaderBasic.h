#pragma once

#include "material/MaterialReader.h"
#include "material/MaterialTypes.h"

namespace WarframeExporter::Material
{
    class MaterialReaderBasic : public MaterialReader
    {
    public:
        inline static MaterialReaderBasic* getInstance()
		{
			static MaterialReaderBasic* instance = new MaterialReaderBasic();
			return instance;
		}

        inline std::vector<int> getEnumMapKeys() const override
        {
            std::vector<int> extTypes = {
                (int)MaterialType::MATERIAL_203,
                (int)MaterialType::MATERIAL_204,
                (int)MaterialType::MATERIAL_205,
                (int)MaterialType::MATERIAL_206,
                (int)MaterialType::MATERIAL_208
            };
			return extTypes;
        }
        
        MaterialExternal readData(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader) override;
    };
};