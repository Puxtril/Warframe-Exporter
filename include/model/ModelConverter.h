#pragma once

#include "ModelStructs.hpp"
#include "ModelReader.h"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/quaternion.hpp"

#include <cstdint>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
#include <unordered_map>

namespace WarframeExporter::Model
{
	class ModelConverter
	{
	public:
		static void convertToInternal(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, std::vector<std::vector<glm::u8vec4>> vertexColors, ModelHeaderInternal& outHeader, ModelBodyInternal& outBody, ScaleType scaleType);
		static void mirrorX(ModelHeaderInternal& intHeader, ModelBodyInternal& intBody);

	private:	
		static void convertInternalHeaderRigged(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelHeaderInternal& outHeader);
		static void convertInternalHeaderStaticOrRigged(ModelHeaderExternal& extHeader, const std::string& attributes, ModelHeaderInternal& outHeader);
		static void convertInternalBodyStaticOrRigged(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, std::vector<std::vector<glm::u8vec4>> vertexColors, const glm::vec4& modelScale);
		static void addVertexColors(ModelBodyInternal& outBody, std::vector<std::vector<glm::u8vec4>> vertexColors);

		static std::vector<std::string> extractMaterialNames(const std::string& attributes);
		static void getModelScale(const std::vector<MeshInfoExternal>& meshInfos, ScaleType scaleType, glm::vec4& outScale);
		static void getModelScale(const std::vector<MeshInfoExternal>& meshInfos, glm::vec4 ensmall1, glm::vec4 ensmall2,  ScaleType scaleType, glm::vec4& outScale);
	};
}
