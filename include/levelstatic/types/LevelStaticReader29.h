#pragma once

#include "levelstatic/LevelStaticReader.h"
#include "levelstatic/LevelStaticTypes.h"

namespace WarframeExporter::LevelStatic
{
    class LevelStaticReader29 : public LevelStaticReader
    {
    public:
        inline static LevelStaticReader29* getInstance()
		{
			static LevelStaticReader29* instance = new LevelStaticReader29();
			return instance;
		}

        inline std::vector<int> getEnumMapKeys() const override
        {
            std::vector<int> extTypes = {
                (int)LevelStaticType::LEVELSTATIC_29,
            };
			return extTypes;
        }

        void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, LevelStaticHeaderExternal& outHeader) override;
        void readBody(BinaryReader::BinaryReaderBuffered* bodyReader, const LevelStaticHeaderExternal& extHeader, LevelStaticBodyExternal& outBody) override;
    };
};