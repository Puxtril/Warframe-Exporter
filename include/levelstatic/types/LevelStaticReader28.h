#pragma once

#include "levelstatic/LevelStaticReader.h"
#include "levelstatic/LevelStaticTypes.h"

namespace WarframeExporter::LevelStatic
{
    class LevelStaticReader28 : public LevelStaticReader
    {
    public:
        inline static LevelStaticReader28* getInstance()
		{
			static LevelStaticReader28* instance = new LevelStaticReader28();
			return instance;
		}

        inline std::vector<int> getEnumMapKeys() const override
        {
            std::vector<int> extTypes = {
                (int)LevelStaticType::LEVELSTATIC_28,
            };
			return extTypes;
        }

        void readHeader(BinaryReader::Buffered* headerReader, LevelStaticHeaderExternal& outHeader) override;
        void readBody(BinaryReader::Buffered* bodyReader, const LevelStaticHeaderExternal& extHeader, LevelStaticBodyExternal& outBody) override;
    };
};