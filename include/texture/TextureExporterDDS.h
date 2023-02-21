#pragma once

#include <fstream>

#include "ddspp.h"

namespace WarframeExporter::Texture
{
	struct DDSHeaderFull
	{
		const unsigned int magic = 542327876;
		ddspp::Header header;
		ddspp::HeaderDXT10 header10;
	};

	//! Wrapper for ddspp library
	class DDSLib
	{
	public:
		static DDSHeaderFull encodeHeader(ddspp::DXGIFormat format, int width, int height);
		static void serialize(std::ostream& outStream, DDSHeaderFull& headerFull);
	};
}
