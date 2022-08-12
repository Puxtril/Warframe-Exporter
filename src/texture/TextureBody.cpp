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
		return TextureBodyInternal(bodyReader->getPtr(), bodyReader->getLength(), true);
	}
	else
	{
		char* rawData = (char*)bodyReader->readUInt8Array(bodyReader->getLength());
		char* unSwizzled = new char[bodyReader->getLength()];
		headerInternal.getFormatClass()->unSwizzle(rawData, bodyReader->getLength(), unSwizzled);
		delete[] rawData;
		return TextureBodyInternal(unSwizzled, bodyReader->getLength(), false);
	}
}