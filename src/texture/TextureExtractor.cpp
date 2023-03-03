#include "texture/TextureExtractor.h"

using namespace WarframeExporter::Texture;

TextureExtractor*
TextureExtractor::getInstance()
{
	static TextureExtractor* instance = new TextureExtractor();
	return instance;
}

void
TextureExtractor::writeData(const std::filesystem::path& outputFile, const TextureHeaderInternal& header, const TextureBodyInternal& body, const LotusLib::CommonHeader& comHeader)
{
	std::ofstream out;
	out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
	
	DDSHeaderFull headerFull = DDSLib::encodeHeader(header.formatClass->getFormat(), header.width, header.height);
	DDSLib::serialize(out, headerFull);

	uint32_t mip0Start = (uint32_t)body.dataLen - header.mip0Len;
	out.write(body.data.get() + mip0Start, header.mip0Len);
	out.close();
}

void
TextureExtractor::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	// Typically, textures above 256x256 are in F. Textures below are in B
	LotusLib::PackageTrioType bodyTrioType = LotusLib::PackageTrioType::F;
	const LotusLib::FileEntries::FileNode* entry;
	try
	{
		entry = pkgDir[package][bodyTrioType]->getFileEntry(internalPath);
	}
	catch (std::filesystem::filesystem_error&)
	{
		bodyTrioType = LotusLib::PackageTrioType::B;
		entry = pkgDir[package][bodyTrioType]->getFileEntry(internalPath);
	}

	std::unique_ptr<char[]> rawData = pkgDir[package][bodyTrioType]->getDataAndDecompress(entry);
	BinaryReaderBuffered reader((uint8_t*)rawData.release(), entry->getLen());

	// Read header
	TextureHeaderExternal extHeader = TextureReader::readHeader(hReader, ensmalleningData);
	TextureHeaderInternal headerInt = TextureConverter::convertHeader(extHeader, entry->getLen());

	m_logger.debug(spdlog::fmt_lib::format("Raw texture data: Format={} Resolution={}x{} Enum1={} Enum2={} Enum3={}", extHeader.format, extHeader.widthBase, extHeader.heightBase, extHeader.unkEnum1, extHeader.unkEnum2, extHeader.unkEnum3));
	m_logger.debug(spdlog::fmt_lib::format("Converted texture data: Resolution={}x{} Mip0Size={}", headerInt.width, headerInt.height, headerInt.mip0Len));

	// Read body
	TextureBodyInternal body = TextureReader::readBody(&reader, headerInt, ensmalleningData);

	writeData(outputPath, headerInt, body, header);
}

void
TextureExtractor::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalPath, const Ensmallening& ensmalleningData)
{
	/*
	BinaryReaderBuffered* fReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::F, internalpath);
	BinaryReaderBuffered* bReader = pkgDir.getFileReader(package, PackageReader::PackageTrioType::B, internalpath);

	TextureHeaderInternal headerInt = readHeader(hReader, fReader, bReader, ensmalleningData, header, m_logger);
	TextureBodyInternal body = readBody(fReader, bReader, ensmalleningData, header, headerInt, m_logger);
	*/
}
