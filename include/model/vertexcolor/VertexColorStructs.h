#pragma once

#include <string>

namespace WarframeExporter::Model::VertexColor
{
	struct VertexColorHeader
	{
		int colorCount;
		std::string modelPath;
	};
}