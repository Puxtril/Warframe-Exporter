#pragma once

#include "../Ensmallening.hpp"
#include "BinaryReaderBuffered.h"
#include "TextureStructs.hpp"

namespace WarframeExporter::Texture
{
	class TextureReader
	{
	public:
		static BinaryReader::BinaryReaderBuffered* getCorrectBodyReader(BinaryReader::BinaryReaderBuffered* FfileReader, BinaryReader::BinaryReaderBuffered* BfileReader);
		static TextureHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData);
		static TextureBodyInternal readBody(BinaryReader::BinaryReaderBuffered* bodyReader, const TextureHeaderInternal& headerInternal, const Ensmallening& postEnsmallening);
	};
}