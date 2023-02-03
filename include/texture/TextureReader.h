#pragma once

#include "../Ensmallening.hpp"
#include "BinaryReaderBuffered.h"
#include "TextureStructs.hpp"

namespace WarframeExporter::Texture
{
	class TextureReader
	{
	public:
		static BinaryReaderBuffered* getCorrectBodyReader(BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader);
		static TextureHeaderExternal readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData);
		static TextureBodyInternal readBody(BinaryReaderBuffered* bodyReader, const TextureHeaderInternal& headerInternal, const Ensmallening& postEnsmallening);
	};
}