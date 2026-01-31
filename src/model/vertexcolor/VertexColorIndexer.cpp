#include "model/vertexcolor/VertexColorIndexer.h"

using namespace WarframeExporter::Model::VertexColor;

VertexColorIndexer::VertexColorIndexer()
	: m_cancelIndexing(false), m_logger(Logger::getInstance())
{}

void
VertexColorIndexer::getModelColors(const std::string& modelPath, std::vector<vertexColorData>& outColors, LotusLib::Package& pkg)
{
	if (!isIndexed(pkg))
	{
		m_logger.info("Indexing vertex colors, this will take some time...");
		indexColors(pkg);
	}

	modelToColorList& colorList = getColorList(pkg);
	std::vector<std::string>& vcPaths = colorList[modelPath];

	size_t curVcArrayIndex = outColors.size();
	outColors.resize(outColors.size() + vcPaths.size());

	for (const std::string& vcPath : vcPaths)
	{
		readColor(pkg, vcPath, outColors[curVcArrayIndex++]);
	}
}

void
VertexColorIndexer::indexVertexColors(LotusLib::Package& pkg)
{
	if (!isIndexed(pkg))
		indexColors(pkg);
}

void
VertexColorIndexer::cancelIndexing()
{
	m_cancelIndexing = true;
}

int
VertexColorIndexer::indexColors(LotusLib::Package& pkg)
{
	modelToColorList& curColorList = getColorList(pkg);

	for (auto& curFile : pkg)
	{
		if (m_cancelIndexing)
		{
			m_cancelIndexing = false;
			break;
		}

		// Sometimes the source path doesn't exist
		// Which means the vertex color is useless...
		// /Lotus/Objects/Tenno/Ships/PlayerShip/Structural/OperatorChairRoom/AirlockTransitionWallAB.vc
		//
		// Also since we're iterating over each file, we will hit uncompressed files
		// Just skip these.
		try
		{
			VertexColorReader* vertexColorReader = g_enumMapVertexColor[pkg.readCommonHeaderFormat(curFile)];
			if (vertexColorReader == nullptr)
				continue;

			VertexColorHeader vertexColorHeader;
			BinaryReader::Buffered headerReader = pkg.getFile(LotusLib::PkgSplitType::HEADER, curFile);
			
			headerReader.seek(LotusLib::commonHeaderFindLen(headerReader, pkg.getGame()), std::ios::beg);
			vertexColorReader->readHeader(&headerReader, vertexColorHeader);

			curColorList[vertexColorHeader.modelPath].push_back(LotusLib::getFullPath(curFile));
		}
		catch (std::exception&)
		{
			continue;
		}
	}
	return static_cast<int>(m_vertexColorMap.size());
}

void
VertexColorIndexer::readColor(LotusLib::Package& pkg, std::string vertexColorPath, vertexColorData& outData)
{
	try
	{
		LotusLib::FileNode curFileNode = pkg.getFileNode(LotusLib::PkgSplitType::HEADER, vertexColorPath);
		BinaryReader::Buffered bodyReader(pkg.getFile(LotusLib::PkgSplitType::BODY, curFileNode));

		VertexColorReader* vertexColorReader = g_enumMapVertexColor[pkg.readCommonHeaderFormat(curFileNode)];
		vertexColorReader->readBody(&bodyReader, outData);
	}
	catch (LotusLib::LotusException&)
	{
		return;
	}
}

bool
VertexColorIndexer::isIndexed(LotusLib::Package& pkg)
{
	const std::filesystem::path& cachePath = pkg.getDirectory();

	if (m_vertexColorMap.count(cachePath) == 1)
		return true;
	return false;
}

VertexColorIndexer::modelToColorList&
VertexColorIndexer::getColorList(LotusLib::Package& pkg)
{
	const std::filesystem::path& cachePath = pkg.getDirectory();
	return m_vertexColorMap[cachePath];
}