#include "texture/TextureExporterDDS.h"

using namespace WarframeExporter::Texture;

DDSHeaderFull 
DDSLib::encodeHeader(ddspp::DXGIFormat format, int width, int height)
{
	DDSHeaderFull ret;
	ddspp::encode_header(
		format,
		width, height,
		1,
		ddspp::Texture2D,
		1,
		1,
		ret.header,
		ret.header10
	);

	// PUXTRIL
	ret.header.reserved1[0] = 1415075152;
	ret.header.reserved1[1] = 4999506;

	return ret;
}

void
DDSLib::serialize(std::ostream& outStream, DDSHeaderFull& headerFull)
{
	outStream.write((char*)&headerFull.magic, 4);
	outStream.write((char*)&headerFull.header.size, 4);
	outStream.write((char*)&headerFull.header.flags, 4);
	outStream.write((char*)&headerFull.header.height, 4);
	outStream.write((char*)&headerFull.header.width, 4);
	outStream.write((char*)&headerFull.header.pitchOrLinearSize, 4);
	outStream.write((char*)&headerFull.header.depth, 4);
	outStream.write((char*)&headerFull.header.mipMapCount, 4);
	outStream.write((char*)&headerFull.header.reserved1, 4 * 11);
	outStream.write((char*)&headerFull.header.ddspf.size, 4);
	outStream.write((char*)&headerFull.header.ddspf.flags, 4);
	outStream.write((char*)&headerFull.header.ddspf.fourCC, 4);
	outStream.write((char*)&headerFull.header.ddspf.RGBBitCount, 4);
	outStream.write((char*)&headerFull.header.ddspf.RBitMask, 4);
	outStream.write((char*)&headerFull.header.ddspf.GBitMask, 4);
	outStream.write((char*)&headerFull.header.ddspf.BBitMask, 4);
	outStream.write((char*)&headerFull.header.ddspf.ABitMask, 4);
	outStream.write((char*)&headerFull.header.caps, 4);
	outStream.write((char*)&headerFull.header.caps2, 4);
	outStream.write((char*)&headerFull.header.caps3, 4);
	outStream.write((char*)&headerFull.header.caps4, 4);
	outStream.write((char*)&headerFull.header.reserved2, 4);

	if (ddspp::is_dxt10(headerFull.header))
	{
		outStream.write((char*)&headerFull.header10.dxgiFormat, 4);
		outStream.write((char*)&headerFull.header10.resourceDimension, 4);
		outStream.write((char*)&headerFull.header10.miscFlag, 4);
		outStream.write((char*)&headerFull.header10.arraySize, 4);
		outStream.write((char*)&headerFull.header10.miscFlags2, 4);
	}
}