#pragma once

#include "BinaryReaderBuffered.h"
#include "../Ensmallening.hpp"
#include "CommonHeader.h"
#include "ExporterExceptions.h"
#include "AnimationStructs.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "EnumMapValue.h"

#include <cassert>
#include <unordered_map>
#include <iostream>
#include <cmath>

namespace WarframeExporter::Animation
{
	enum class AnimationType
	{
		ANIMATION_132 = 132
	};
	
	class AnimationReader : public EnumMapValue
	{
	protected:
		AnimationReader() = default;
		
	public:
		virtual void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) = 0;
		virtual void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AnimationHeaderExternal& outHeader) = 0;
		virtual void readBodyDebug(BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) = 0;
		virtual void readBody(BinaryReaderBuffered* bodyReader, const AnimationHeaderExternal& extHeader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, AnimationBodyExternal& outBody) = 0;
	};
}