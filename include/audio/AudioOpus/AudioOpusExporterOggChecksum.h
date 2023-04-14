#pragma once

#include <cstdint>
#include <type_traits>

#include <string>
#include <iostream>

// Based on code from Python's crcmod module. Polynomial "crc-32".
// Taken from https://gist.github.com/iwanders/8e1cb7b92af2ccf8d1a73450d771f483`
namespace WarframeExporter::Audio
{
	class AudioOpusExporterOggChecksum
	{
	public:
		static const uint32_t table[][256];
	
		static uint32_t calculateHash(const unsigned char* data, uint32_t len, uint32_t crc = 0);
	};
}
