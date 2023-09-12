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

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)AnimationType::ANIMATION_132 };
			return extTypes;
		}
		
		void isPureChannelType(uint16_t* channelTypes, int channelCount);
		void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) override;
		void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AnimationHeaderExternal& outHeader) override;
		void readBodyDebug(BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) override;
		void readBody(BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AnimationBodyExternal& outBody) override;
	};
}