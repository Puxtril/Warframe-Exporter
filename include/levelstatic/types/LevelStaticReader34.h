#pragma once

#include "levelstatic/LevelStaticReader.h"
#include "levelstatic/LevelStaticTypes.h"

namespace WarframeExporter::LevelStatic
{
    class LevelStaticReader34 : public LevelStaticReader
    {
    public:
        inline static LevelStaticReader34* getInstance()
		{
			static LevelStaticReader34* instance = new LevelStaticReader34();
			return instance;
		}

        inline std::vector<int> getEnumMapKeys() const override
        {
            std::vector<int> extTypes = {
                (int)LevelStaticType::LEVELSTATIC_34,
            };
			return extTypes;
        }

        void readHeader(BinaryReader::Buffered* headerReader, LevelStaticHeaderExternal& outHeader) override;
        void readBody(BinaryReader::Buffered* bodyReader, const LevelStaticHeaderExternal& extHeader, LevelStaticBodyExternal& outBody) override;
    };
};