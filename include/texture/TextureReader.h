#pragma once

#include "BinaryReaderBuffered.h"
#include "LotusLib.h"
#include "TextureStructs.hpp"

namespace WarframeExporter::Texture
{
	class TextureReader
	{
	public:
		static BinaryReader::BinaryReaderBuffered* getCorrectBodyReader(BinaryReader::BinaryReaderBuffered* FfileReader, BinaryReader::BinaryReaderBuffered* BfileReader);
		static TextureHeaderExternal readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& commonHeader);
		static std::vector<char> readBody(BinaryReader::BinaryReaderBuffered* bodyReader, const TextureHeaderExternal& headerExternal);
	};
}