#include "level/LevelExtractor.h"

using namespace WarframeExporter::Level;

LevelExtractor*
LevelExtractor::getInstance()
{
	static LevelExtractor* instance = new LevelExtractor();
	return instance;
}

void
LevelExtractor::extract(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData, const std::filesystem::path& outputPath)
{
	const LotusLib::FileEntries::FileNode* bEntry = pkgDir[package][LotusLib::PackageTrioType::B]->getFileEntry(internalpath);
	std::unique_ptr<char[]> bRawData = pkgDir[package][LotusLib::PackageTrioType::B]->getDataAndDecompress(bEntry);
	BinaryReaderBuffered bReader = BinaryReaderBuffered((uint8_t*)bRawData.release(), bEntry->getLen());

	LevelReader* levelReader = g_enumMapLevel[header.type];
	
	LevelHeaderExternal headerExt;
	LevelBodyExternal bodyExt;
	levelReader->readHeader(*hReader, headerExt);
	levelReader->readBody(bReader, headerExt, bodyExt);

	LevelInternal bodyInt;
	LevelConverter::convertToInternal(headerExt, bodyExt, bodyInt);
	m_logger.info("Level mesh count: " + std::to_string(bodyInt.objs.size()));

	LevelExporterGltf gltfOut;

	pkgDir["Misc"][LotusLib::PackageTrioType::H]->readToc();
	pkgDir["Misc"][LotusLib::PackageTrioType::B]->readToc();
	createGltfCombined(pkgDir, ensmalleningData, bodyInt, gltfOut);

	gltfOut.save(outputPath);
}

void
LevelExtractor::extractDebug(const LotusLib::CommonHeader& header, BinaryReaderBuffered* hReader, LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const std::string& package, const LotusLib::LotusPath& internalpath, const Ensmallening& ensmalleningData)
{
	LevelReader* levelReader = g_enumMapLevel[header.type];
	levelReader->readHeaderDebug(*hReader);
}

void
LevelExtractor::createGltfCombined(LotusLib::PackageCollection<LotusLib::CachePairReader>& pkgDir, const Ensmallening& ensmalleningData, LevelInternal& bodyInt, LevelExporterGltf& outGltf)
{
	size_t validModels = 0;
	for (size_t x = 0; x < bodyInt.objs.size(); x++)
	{
		LevelObjectInternal& curLevelObj = bodyInt.objs[x];

		if (curLevelObj.meshPath == "")
			continue;

		try {
			const LotusLib::FileEntries::FileNode* hEntry = pkgDir["Misc"][LotusLib::PackageTrioType::H]->getFileEntry(curLevelObj.meshPath);
			std::unique_ptr<char[]> hRawData = pkgDir["Misc"][LotusLib::PackageTrioType::H]->getDataAndDecompress(hEntry);
			BinaryReaderBuffered hReader = BinaryReaderBuffered((uint8_t*)hRawData.release(), hEntry->getLen());

			LotusLib::CommonHeader commonHeader;
			int headerLen = LotusLib::CommonHeaderReader::readHeader(hReader.getPtr(), commonHeader);
			hReader.seek(headerLen, std::ios::beg);

			WarframeExporter::Model::ModelHeaderExternal headerExt;
			WarframeExporter::Model::ModelBodyExternal bodyExt;
			WarframeExporter::Model::ModelExtractor::getInstance()->extractExternal(commonHeader, &hReader, pkgDir, "Misc", curLevelObj.meshPath, ensmalleningData, headerExt, bodyExt);

			// My Gltf exporter doesn't like multiple rigged models
			// These are levels anyway, so no big deal right?
			// ...
			// Right?
			headerExt.boneTree.clear();

			WarframeExporter::Model::ModelHeaderInternal headerInt;
			WarframeExporter::Model::ModelBodyInternal bodyInt;
			WarframeExporter::Model::ModelConverter::convertToInternal(headerExt, bodyExt, commonHeader.attributes, headerInt, bodyInt);

			outGltf.addModelData(headerInt, bodyInt, curLevelObj);
		
			validModels++;
		} catch (std::exception& ex) {
			if (curLevelObj.meshPath.length() > 5)
				m_logger.error(spdlog::fmt_lib::format("{}::{}: {}", curLevelObj.meshPath, curLevelObj.objName, ex.what()));
			continue;
		}
	}
}