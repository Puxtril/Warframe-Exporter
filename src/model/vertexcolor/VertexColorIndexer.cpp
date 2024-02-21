#include "model/vertexcolor/VertexColorIndexer.h"
#include "LotusLib.h"

using namespace WarframeExporter::Model::VertexColor;

VertexColorIndexer::VertexColorIndexer()
	: m_logger(Logger::getInstance())
{}

void
VertexColorIndexer::getModelColors(const LotusLib::LotusPath& modelPath, std::vector<vertexColorData>& outColors, LotusLib::PackageReader& pkg)
{
	if (!isIndexed(pkg))
	{
		m_logger.info("Indexing vertex colors, this will take some time...");
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
VertexColorIndexer::indexColors(LotusLib::PackageReader& pkg)
{
	modelToColorList& curColorList = getColorList(pkg);

	for (auto& curFile : pkg)
	{
		// Sometimes the source path doesn't exist
		// Which means the vertex color is useless...
		// /Lotus/Objects/Tenno/Ships/PlayerShip/Structural/OperatorChairRoom/AirlockTransitionWallAB.vc
		//
		// Also since we're iterating over each file, we will hit uncompressed files
		// Just skip these.
		try
		{
			LotusLib::FileEntry curFileEntry = pkg.getFile(curFile, LotusLib::READ_COMMON_HEADER);

			VertexColorReader* vertexColorReader = g_enumMapVertexColor[curFileEntry.commonHeader.type];
			if (vertexColorReader == nullptr)
				continue;

			VertexColorHeader vertexColorHeader;
			vertexColorReader->readHeader(&curFileEntry.headerData, vertexColorHeader);

			curColorList[vertexColorHeader.modelPath].push_back(LotusLib::LotusPath(curFile.getFullPath()));
		}
		catch (std::exception&)
		{
			continue;
		}
	}
	return m_vertexColorMap.size();
}

void
VertexColorIndexer::readColor(LotusLib::PackageReader& pkg, LotusLib::LotusPath vertexColorPath, vertexColorData& outData)
{
	try
	{
		LotusLib::FileEntry curFileEntry = pkg.getFile(vertexColorPath);
		VertexColorReader* vertexColorReader = g_enumMapVertexColor[curFileEntry.commonHeader.type];
		vertexColorReader->readBody(&curFileEntry.bData, outData);
	}
	catch (LotusLib::LotusException&)
	{
		return;
	}
}

bool
VertexColorIndexer::isIndexed(LotusLib::PackageReader& pkg)
{
	const std::filesystem::path& cachePath = pkg.getDirectory();
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
VertexColorIndexer::getColorList(LotusLib::PackageReader& pkg)
{
	const std::filesystem::path& cachePath = pkg.getDirectory();
	return m_vertexColorMap[cachePath];
}