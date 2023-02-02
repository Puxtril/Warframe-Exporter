#include "texture/TextureBody.h"

using namespace WarframeExporter::Texture;

BinaryReaderBuffered*
TextureBody::getCorrectBodyReader(BinaryReaderBuffered* FfileReader, BinaryReaderBuffered* BfileReader)
{
	return FfileReader->getLength() > BfileReader->getLength() ? FfileReader : BfileReader;
}

TextureBodyInternal
TextureBody::getBody(BinaryReaderBuffered* bodyReader, const TextureHeaderInternal& headerInternal, const Ensmallening& postEnsmallening)
{
	if (postEnsmallening.isPostPart1())
	{
		std::unique_ptr<char[]> data = std::make_unique<char[]>(bodyReader->getLength());
		std::memcpy(data.get(), bodyReader->getPtr(), bodyReader->getLength());
		return TextureBodyInternal{ std::move(data), bodyReader->getLength() };
	}
	else
	{
		char* rawData = (char*)bodyReader->readUInt8Array(bodyReader->getLength());
		std::unique_ptr<char[]> unSwizzled = std::make_unique<char[]>(bodyReader->getLength());
		headerInternal.formatClass->unSwizzle(rawData, bodyReader->getLength(), unSwizzled.get());
		delete[] rawData;
		return TextureBodyInternal{std::move(unSwizzled), bodyReader->getLength()};
	}
}
