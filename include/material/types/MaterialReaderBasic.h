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
                (int)MaterialBasicType::MATERIAL_203,
                (int)MaterialBasicType::MATERIAL_204,
                (int)MaterialBasicType::MATERIAL_205,
                (int)MaterialBasicType::MATERIAL_206,
                (int)MaterialBasicType::MATERIAL_208,
                (int)MaterialBasicType::MATERIAL_209,
                (int)MaterialBasicType::MATERIAL_210
            };
			return extTypes;
        }
        
        MaterialExternal readData(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& commonHeader) override;
    };
};