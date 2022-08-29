#pragma once

#include "spdlog/spdlog.h"
#include "BinaryReaderBuffered.h"
#include "FileNode.h"
#include "Package.h"
#include "model/ModelStructs.hpp"
#include "CommonHeader.h"
#include "FileType.hpp"
#include "glm/vec4.hpp"

#include <unordered_map>
#include <vector>
#include <cstdint>

namespace WarframeExporter
{
	namespace Model
	{
		// Constant expected: Vertex colors are only stored in 1 package: Misc
		class VertexColorManager
		{
			struct VertexColorEntry
			{
				uint32_t colorCount;
				const Entries::FileNode* colorNode;
			};

			std::unordered_map<const Entries::FileNode*, std::vector<VertexColorEntry>> m_vertexColorMap;
			PackageReader::Package* m_pkg;
			std::shared_ptr<spdlog::logger> m_logger;

			VertexColorManager();
			VertexColorManager(const VertexColorManager& other) = delete;
			VertexColorManager& operator=(const VertexColorManager& other) = delete;
		
		public:
			static VertexColorManager& getInstance();

			void setPackage(PackageReader::Package* package);

			int indexColors();
			std::vector<VertexColorBody> getModelColors(const Entries::FileNode* modelNode);

		private:
			void readColor(const Entries::FileNode* colorEntry, uint32_t entryCount, VertexColorBody& outColor);
		};
	}
}