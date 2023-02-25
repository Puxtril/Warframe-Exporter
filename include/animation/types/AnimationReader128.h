#pragma once

#include "animation/AnimationReader.h"

namespace WarframeExporter::Animation
{
	class AnimationReader128 : public AnimationReader
	{
		AnimationReader128() = default;

	public:
		inline static AnimationReader128* getInstance()
		{
			static AnimationReader128* instance = new AnimationReader128();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)AnimationType::ANIMATION_128 };
			return extTypes;
		}
	
		void isPureChannelType(uint16_t* channelTypes, int channelCount);
		void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header);
		void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AnimationHeaderExternal& outHeader);
		void readBodyDebug(BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header);
		void readBody(BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AnimationBodyExternal& outBody);
	};
}