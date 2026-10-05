#pragma once

#include "levelstatic/LevelStaticReader.h"
#include "levelstatic/LevelStaticTypes.h"

namespace WarframeExporter::LevelStatic
{
    class LevelStaticReader36 : public LevelStaticReader
    {
    public:
        inline static LevelStaticReader36* getInstance()
		{
			static LevelStaticReader36* instance = new LevelStaticReader36();
			return instance;
		}

        inline std::vector<int> getEnumMapKeys() const override
        {
            std::vector<int> extTypes = {
                (int)LevelStaticType::LEVELSTATIC_36,
            };
			return extTypes;
        }

        void readHeader(BinaryReader::Buffered* headerReader, LevelStaticHeaderExternal& outHeader) override;
        void readBody(BinaryReader::Buffered* bodyReader, const LevelStaticHeaderExternal& extHeader, LevelStaticBodyExternal& outBody) override;
    };
};