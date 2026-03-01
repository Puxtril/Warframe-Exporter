#pragma once

#include "ExporterLogger.h"
#include "BinaryReader/Buffered.h"
#include "LotusLib/Package.h"
#include "model/ModelStructs.hpp"
#include "LotusLib/CommonHeader.h"
#include "glm/vec4.hpp"

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
		using modelToColorList = std::map<std::string, std::vector<std::string>>;

		// A mapping of CachePairs (H.Misc.toc) to their cached vertex colors
		using packageColors = std::map<std::filesystem::path, modelToColorList>;

		using vertexColorData = std::vector<glm::u8vec4>;

		bool m_cancelIndexing;
		packageColors m_vertexColorMap;
		Logger& m_logger;
		
	public:
		VertexColorIndexer();

		void getModelColors(const std::string& modelPath, std::vector<vertexColorData>& outColors, LotusLib::Package& pkg);
		void indexVertexColors(LotusLib::Package& pkg);
		void cancelIndexing();

	private:
		int indexColors(LotusLib::Package& pkg);
		void readColor(LotusLib::Package& pkg, std::string vertexColorPath, vertexColorData& outData);

		bool isIndexed(LotusLib::Package& pkg);

		// Will create if doesn't exist
		modelToColorList& getColorList(LotusLib::Package& pkg);
	};
}