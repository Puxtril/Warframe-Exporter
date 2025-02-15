#include "texture/TextureExtractor.h"

using namespace WarframeExporter::Texture;

TextureExtractor*
TextureExtractor::getInstance()
{
	static TextureExtractor* instance = new TextureExtractor();
	return instance;
}

TextureInternal
TextureExtractor::getTexture(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs)
{
	// Typically, textures above 256x256 are in F. Textures below are in B
	BinaryReader::BinaryReaderBuffered& entry = fileEntry.fData.getLength() != 0 ? fileEntry.fData : fileEntry.bData;

	TextureHeaderExternal extHeader = TextureReader::readHeader(&fileEntry.headerData, fileEntry.commonHeader);

	if (internalFormatToDdsFormat.count(static_cast<TextureCompression>(extHeader.format)) == 0)
		throw std::runtime_error("Unknown texture compression format: " + std::to_string(extHeader.format));

	TextureInternal intTexture;
	intTexture.header = TextureConverter::convertHeader(extHeader, static_cast<int32_t>(entry.getLength()), fileEntry.commonHeader.type);
	intTexture.body = TextureReader::readBody(&entry, extHeader);

	m_logger.debug(spdlog::fmt_lib::format("Format={} ResRaw={}x{} ResConv={}x{} Mip0Size={}", extHeader.format, extHeader.widthBase, extHeader.heightBase, intTexture.header.width, intTexture.header.height, intTexture.header.mip0Len));
	
	return intTexture;
}

void
TextureExtractor::writeData(TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const std::filesystem::path& outputFile)
{
	if (texture.header.textureNames.size() > 0)
	{
		uint32_t mip0Start = (uint32_t)texture.body.size() - (texture.header.mip0Len * texture.header.textureNames.size());
		writeArray(texture, commonHeader, texture.body.data() + mip0Start, texture.header.mip0Len, outputFile);
	}
	else
	{
		uint32_t mip0Start = (uint32_t)texture.body.size() - texture.header.mip0Len;
		writeTextureToFile(texture, commonHeader, texture.body.data() + mip0Start, texture.header.mip0Len, outputFile);
	}
}

void
TextureExtractor::extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, bool dryRun)
{
	TextureInternal intTexture = getTexture(fileEntry, pkgs);
	if (!dryRun)
		writeData(intTexture, fileEntry.commonHeader, outputPath);
}

void
TextureExtractor::writeArray(TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const char* data, size_t dataLen, const std::filesystem::path& outputFile)
{
	std::filesystem::path baseOutputArray = std::filesystem::path(outputFile).replace_extension();
	std::filesystem::create_directories(baseOutputArray);

	const char* dataStart = data;
	for (int i = 0; i < texture.header.textureNames.size(); i++)
	{
		std::filesystem::path outPath = baseOutputArray / (outputFile.stem().string() + "_" + std::to_string(i) + outputFile.extension().string());		
		TextureExtractor::writeTextureToFile(texture, commonHeader, dataStart, dataLen, outPath.string());
		dataStart += dataLen;
	}
}

void
TextureExtractor::writeTextureToFile(TextureInternal& texture, const LotusLib::CommonHeader& commonHeader, const char* data, size_t dataLen, const std::filesystem::path& outputFile)
{
	if (TextureExtractor::m_exportType == TextureExportType::TEXTURE_EXPORT_DDS)
	{
		std::ofstream out;
		out.open(outputFile, std::ios::binary | std::ios::out | std::ofstream::trunc);
		
		DDSHeaderFull headerFull = DDSLib::encodeHeader(texture.header.ddsFormat, texture.header.width, texture.header.height);
		DDSLib::serialize(out, headerFull);

		out.write(data, dataLen);
		out.close();
	}

	else if (texture.header.formatEnum == TextureCompression::BC6)
		TextureExporterConvert::convertAndWriteToHdr(data, dataLen, outputFile, texture.header.width, texture.header.height);

	else if (TextureExtractor::m_exportType == TextureExportType::TEXTURE_EXPORT_PNG)
		TextureExporterConvert::convertAndWriteToPng(data, dataLen, outputFile, texture.header.formatEnum, texture.header.width, texture.header.height);

	else if (TextureExtractor::m_exportType == TextureExportType::TEXTURE_EXPORT_TGA)
		TextureExporterConvert::convertAndWriteToTga(data, dataLen, outputFile, texture.header.formatEnum, texture.header.width, texture.header.height);
}