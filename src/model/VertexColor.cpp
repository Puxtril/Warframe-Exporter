#include "model/VertexColor.h"

using namespace WarframeExporter::Model;

VertexColorManager::VertexColorManager()
	: m_pkg(nullptr), m_logger(spdlog::get("Warframe-Exporter"))
{}

VertexColorManager&
VertexColorManager::getInstance()
{
	static VertexColorManager instance;
	return instance;
}

void
VertexColorManager::setPackage(PackageReader::Package* package)
{
	m_pkg = package;
}

int
VertexColorManager::indexColors()
{
	const PackageReader::CachePair* curPair = (*m_pkg)[PackageReader::PackageTrioType::H];
	for (auto start = curPair->getIteratorRecursive("/"), end = curPair->end(); start != end; ++start)
	{
		const Entries::FileNode* curFile = *start;
		CommonFileHeader header;

		uint32_t colorCount = 0;
		std::string modelPath = "";

		try
		{
			BinaryReaderBuffered rawData(curPair->getDataAndDecompress(curFile), curFile->getLen());
			header = CommonFileHeader(rawData);

			if (header.getEnum() != (int)FileTypeExternal::VERTEX_COLOR)
				continue;

			colorCount = rawData.readUInt32();
			int pathLen = rawData.readUInt32();
			modelPath = rawData.readAsciiString(pathLen);
		}
		catch (std::exception& ex)
		{
			m_logger->debug(ex.what());
			continue;
		}

		// Sometimes the source path doesn't exist
		// Which means the vertex color is useless...
		// /Lotus/Objects/Tenno/Ships/PlayerShip/Structural/OperatorChairRoom/AirlockTransitionWallAB.vc
		try {
			const Entries::FileNode* binaryNode = (*m_pkg)[PackageReader::PackageTrioType::B]->getFileEntry(curFile->getFullPath());
			m_vertexColorMap[modelPath].push_back({colorCount, binaryNode});
		}
		catch (std::exception& ex)
		{
			continue;
		}

	}

	return m_vertexColorMap.size();
}

std::vector<VertexColorBody>
VertexColorManager::getModelColors(const std::string& modelPath)
{
	auto colorNodes = m_vertexColorMap[modelPath];

	std::vector<VertexColorBody> readColors(colorNodes.size());
	for (int x = 0; x < colorNodes.size(); x++)
	{
		readColor(colorNodes[x].colorNode, colorNodes[x].colorCount, readColors[x]);
	}

	return readColors;
}

void
VertexColorManager::readColor(const Entries::FileNode* colorEntry, uint32_t entryCount, VertexColorBody& outColor)
{
	const PackageReader::CachePair* curPair = (*m_pkg)[PackageReader::PackageTrioType::B];
	BinaryReaderBuffered rawData(curPair->getDataAndDecompress(colorEntry), colorEntry->getLen());

	std::vector<glm::u8vec4> readColors(entryCount);
	
	for (uint32_t x = 0; x < entryCount; x++)
	{
		rawData.readUInt8Array(&readColors[x][0], 4);
	}

	outColor.setColors(readColors);
}