#pragma once 

#include "BinaryReaderBase.h"

#include <string>
#include <vector>
#include <cstdint>

namespace WarframeExporter
{
	struct CommonFileHeader {
	private:
		uint8_t hash[16];
		std::vector<std::string> sourcePaths;
		std::string attributes;
		uint32_t typeEnum;

	public:
		CommonFileHeader() = default;
		CommonFileHeader(BinaryReaderBase& reader);

		const char* getHash() const;
		const std::vector<std::string> getPaths() const;
		const std::string& getAttributes() const;
		uint32_t getEnum() const;
	};
}