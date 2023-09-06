#include "model/vertexcolor/VertexColorIndexer.h"

using namespace WarframeExporter::Model::VertexColor;

VertexColorIndexer::VertexColorIndexer()
	: m_logger(Logger::getInstance())
{}

void
VertexColorIndexer::getModelColors(const LotusLib::LotusPath& modelPath, std::vector<vertexColorData>& outColors, LotusLib::Package<LotusLib::CachePairReader>& pkg)
{
	if (!isIndexed(pkg))
	{
		m_logger.info("Indexing vertex colors");
		indexColors(pkg);
	}

	modelToColorList& colorList = getColorList(pkg);
	std::vector<LotusLib::LotusPath>& vcPaths = colorList[modelPath.string()];

	size_t curVcArrayIndex = outColors.size();
	outColors.resize(outColors.size() + vcPaths.size());

	for (const LotusLib::LotusPath& vcPath : vcPaths)
	{
		readColor(pkg, vcPath, outColors[curVcArrayIndex++]);
	}
}

int
VertexColorIndexer::indexColors(LotusLib::Package<LotusLib::CachePairReader>& pkg)
{
	std::shared_ptr<LotusLib::CachePairReader> curPair = pkg[LotusLib::PackageTrioType::H];

	modelToColorList& curColorList = getColorList(pkg);

	for (auto start = curPair->getIter("/"), end = curPair->getIter(); start != end; ++start)
	{
		const LotusLib::FileEntries::FileNode* curFile = *start;

		// Sometimes the source path doesn't exist
		// Which means the vertex color is useless...
		// /Lotus/Objects/Tenno/Ships/PlayerShip/Structural/OperatorChairRoom/AirlockTransitionWallAB.vc
		//
		// Also since we're iterating over each file, we will hit uncompressed files
		// Just skip these.
		try
		{
			LotusLib::CommonHeader header;
			std::unique_ptr<char[]> hRawData = curPair->getDataAndDecompress(curFile);
			BinaryReaderBuffered rawDataReader((uint8_t*)hRawData.release(), curFile->getLen());

			int headerLen = LotusLib::CommonHeaderReader::readHeader(rawDataReader.getPtr(), header);
			rawDataReader.seek(headerLen, std::ios::beg);

			VertexColorReader* vertexColorReader = g_enumMapVertexColor[header.type];
			if (vertexColorReader == nullptr)
				continue;

			VertexColorHeader vertexColorHeader;
			vertexColorReader->readHeader(&rawDataReader, vertexColorHeader);

			pkg[LotusLib::PackageTrioType::B]->getFileEntry(vertexColorHeader.modelPath);
			curColorList[vertexColorHeader.modelPath].push_back(LotusLib::LotusPath(curFile->getFullPath()));
		}
		catch (std::exception&)
		{
			continue;
		}
	}

	return m_vertexColorMap.size();
}

void
VertexColorIndexer::readColor(LotusLib::Package<LotusLib::CachePairReader>& pkg, LotusLib::LotusPath vertexColorPath, vertexColorData& outData)
{
	const LotusLib::FileEntries::FileNode* curFile = pkg[LotusLib::PackageTrioType::H]->getFileEntry(vertexColorPath);

	LotusLib::CommonHeader header;
	std::unique_ptr<char[]> hRawData = pkg[LotusLib::PackageTrioType::H]->getDataAndDecompress(curFile);
	BinaryReaderBuffered rawHDataReader((uint8_t*)hRawData.release(), curFile->getLen());
	try
	{
		int headerLen = LotusLib::CommonHeaderReader::readHeader(rawHDataReader.getPtr(), header);
		rawHDataReader.seek(headerLen, std::ios::beg);
	}
	catch (LotusLib::LotusException&)
	{
		return;
	}

	const LotusLib::FileEntries::FileNode* curFileB = pkg[LotusLib::PackageTrioType::B]->getFileEntry(vertexColorPath);
	std::unique_ptr<char[]> bRawData = pkg[LotusLib::PackageTrioType::B]->getDataAndDecompress(curFileB);
	BinaryReaderBuffered rawBDataReader((uint8_t*)bRawData.release(), curFileB->getLen());
	
	VertexColorReader* vertexColorReader = g_enumMapVertexColor[header.type];

	vertexColorReader->readBody(&rawBDataReader, outData);
}

bool
VertexColorIndexer::isIndexed(LotusLib::Package<LotusLib::CachePairReader>& pkg)
{
	const std::filesystem::path& cachePath = pkg[LotusLib::PackageTrioType::B]->getTocPath();
	try
	{
		m_vertexColorMap.at(cachePath);
		return true;
	}
	catch (std::out_of_range&)
	{
		return false;
	}
}

VertexColorIndexer::modelToColorList&
VertexColorIndexer::getColorList(LotusLib::Package<LotusLib::CachePairReader>& pkg)
{
	const std::filesystem::path& cachePath = pkg[LotusLib::PackageTrioType::B]->getTocPath();
	return m_vertexColorMap[cachePath];
}