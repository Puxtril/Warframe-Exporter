#pragma once

#include "animation/AnimationReader.h"

#include <bitset>

namespace WarframeExporter::Animation
{
	class AnimationReader132 : public AnimationReader
	{
	public:
		inline static AnimationReader132* getInstance()
		{
			static AnimationReader132* instance = new AnimationReader132();
			return instance;
		}

		inline std::vector<std::tuple<LotusLib::Game, int>> getEnumMapKeys() const override
		{
			std::vector<std::tuple<LotusLib::Game, int>> extTypes = {
				{ LotusLib::Game::WARFRAME, (int)AnimationType::ANIMATION_132 },
			};
			return extTypes;
		}
		
		// Purely for debugging purposes
		// When researching channel types, it's helpful to know what animations have many of 1 type
		void isPureChannelType(uint16_t* channelTypes, int channelCount);

		void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, AnimationHeaderExternal& outHeader) override;
		void readBody(BinaryReader::BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const LotusLib::CommonHeader& header, AnimationBodyExternal& outBody) override;

	protected:
		/*
			[    Short 1    ] [    Short 2    ] [    Short 3    ]
			DAAAAAAA AAAAAAAE GHBBBBBB BBBBBBBB CCCCCCCC CCCCCCJJ

			A: Split 15-bit signed integer
			B: Split 15-bit signed integer
			C: Split 15-bit signed integer
			D: A signed bit
			E: C signed bit
			G: Largest integer signed bit
			H: B signed bit
			J: Largest integer index
		*/
		glm::quat unpackQuaternion(uint16_t rawA, uint16_t rawB, uint16_t rawC);
	};
}