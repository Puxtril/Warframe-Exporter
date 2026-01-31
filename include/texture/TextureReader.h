#pragma once

#include "BinaryReader/Buffered.h"
#include "LotusLib/CommonHeader.h"
#include "TextureStructs.hpp"

namespace WarframeExporter::Texture
{
	class TextureReader
	{
	public:
		static BinaryReader::Buffered* getCorrectBodyReader(BinaryReader::Buffered* FfileReader, BinaryReader::Buffered* BfileReader);
		static TextureHeaderExternal readHeader(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& commonHeader);
		static std::vector<char> readBody(BinaryReader::Buffered* bodyReader, const TextureHeaderExternal& headerExternal);
	};
}