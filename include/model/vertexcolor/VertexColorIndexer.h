#pragma once

#include "ExporterLogger.h"
#include "BinaryReaderBuffered.h"
#include "FileNode.h"
#include "LotusLib.h"
#include "Package.h"
#include "model/ModelStructs.hpp"
#include "CommonHeader.h"
#include "glm/vec4.hpp"
#include "LotusPath.h"
#include "model/vertexcolor/VertexColorEnumMap.h"

#include <map>
#include <vector>
#include <cstdint>
#include <filesystem>
#include <iostream>

namespace WarframeExporter::Model::VertexColor
{
	// Constant expected: Vertex colors are only stored in 1 package: Misc
	class VertexColorIndexer
	{
		// A mapping of a LotusPath (model) to LotusPaths (vertex colors)
		using modelToColorList = std::map<std::string, std::vector<LotusLib::LotusPath>>;

		// A mapping of CachePairs (H.Misc.toc) to their cached vertex colors
		using packageColors = std::map<std::filesystem::path, modelToColorList>;

		using vertexColorData = std::vector<glm::u8vec4>;

		bool m_cancelIndexing;
		packageColors m_vertexColorMap;
		Logger& m_logger;
		
	public:
		VertexColorIndexer();

		void getModelColors(const LotusLib::LotusPath& modelPath, std::vector<vertexColorData>& outColors, LotusLib::PackageReader& pkg);
		void indexVertexColors(LotusLib::PackageReader& pkg);
		void cancelIndexing();

	private:
		int indexColors(LotusLib::PackageReader& pkg);
		void readColor(LotusLib::PackageReader& pkg, LotusLib::LotusPath vertexColorPath, vertexColorData& outData);

		bool isIndexed(LotusLib::PackageReader& pkg);

		// Will create if doesn't exist
		modelToColorList& getColorList(LotusLib::PackageReader& pkg);
	};
}