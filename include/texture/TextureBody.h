#pragma once

#include "../Ensmallening.hpp"
#include "BinaryReaderBuffered.h"
#include "TextureStructs.hpp"

namespace WarframeExporter::Texture
{
	class TextureBody
	{
	public:
		static BinaryReaderBuffered* getCorrectBodyReader(BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader);
		static TextureBodyInternal getBody(BinaryReaderBuffered* bodyReader, const TextureHeaderInternal& headerInternal, const Ensmallening& postEnsmallening);
	};
}