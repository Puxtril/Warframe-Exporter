#pragma once

#include "ModelStructs.hpp"
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

namespace WarframeExporter
{
	namespace Model
	{
		class ModelConverter
		{
		public:
			static void convertToInternal(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, std::vector<VertexColorBody> vColors, ModelHeaderInternal& outHeader, ModelBodyInternal& outBody);

		private:
			//static void convertToInternalHeader(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, ModelHeaderInternal& outHeader);
			//static void convertToInternalBody(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, const glm::vec3& modelScale);
	
			static void convertInternalHeaderRigged(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, ModelHeaderInternal& outHeader);
			static void convertInternalHeaderStaticOrRigged(ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, const std::string& attributes, ModelHeaderInternal& outHeader);
			static void convertInternalBodyRigged(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, const glm::vec3& modelScale);
			static void convertInternalBodyStaticOrRigged(const ModelHeaderExternal& extHeader, ModelBodyExternal& extBody, ModelBodyInternal& outBody, const glm::vec3& modelScale, std::vector<VertexColorBody> vColors);

			static void overwriteColors(std::vector<glm::u8vec4>& colors);
			static void flipXAxis(ModelBodyExternal& extBody);
			static std::vector<std::string> extractMaterialNames(const std::string& attributes);
			static void getModelScale(const std::vector<MeshInfoExternal>& meshInfos, glm::vec3& outScale);
		};
	}
}
