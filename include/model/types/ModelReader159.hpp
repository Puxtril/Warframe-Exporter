#pragma once

#include "BinaryReaderExceptions.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model/ModelReader.h"
#include "ExporterExceptions.h"

#include <cassert>
#include <iomanip>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace WarframeExporter::Model
{
	class ModelReader159 : public ModelReader
	{
		ModelReader159() = default;

	public:
		inline static ModelReader159* getInstance()
		{
			static ModelReader159* instance = new ModelReader159();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)ModelType::MODEL_TERRAIN_159 };
			return extTypes;
		}

		inline bool needsEnsmalleningScale() const override
		{
			return false;
		}

		void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) override
		{
			uint32_t strLen = headerReader->readUInt32();
			headerReader->seek(strLen, std::ios::cur);

			uint32_t strLen2 = headerReader->readUInt32();
			headerReader->seek(strLen2, std::ios::cur);

			headerReader->seek(8, std::ios::cur);

			// 6 floats
			headerReader->seek(4 * 6, std::ios::cur);

			static const std::string faceCountMaxMsg = "UInt64 face count";
			uint64_t faceCount = headerReader->readUInt64(0, UINT32_MAX, faceCountMaxMsg);

			const static std::string meshInfoMsg = "Mesh Info Count";
			uint32_t meshInfoCount = headerReader->readUInt32(1, 5, meshInfoMsg);

			for (uint32_t x = 0; x < meshInfoCount; x++)
			{
				headerReader->seek(0x20, std::ios_base::cur);

				uint32_t meshInfoNameLen = headerReader->readUInt32();
				headerReader->seek(meshInfoNameLen, std::ios_base::cur);

				auto meshInfoOffset = headerReader->tell();

				// Apparently these can have less than 1 LOD
				// Took out some debug logic because these are all over the place
				static const std::string LODOffsetMsg = "MeshInfo LOD Offset";
				headerReader->readUInt32();
				headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);
				headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);
				headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);
				headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);

				uint32_t curFaceLODCount = headerReader->readUInt32();
				curFaceLODCount = headerReader->readUInt32();
				curFaceLODCount = headerReader->readUInt32();
				curFaceLODCount = headerReader->readUInt32();
				curFaceLODCount = headerReader->readUInt32();

				headerReader->seek(0x20, std::ios::cur);

				uint32_t strLen = headerReader->readUInt32();
				headerReader->seek(strLen, std::ios::cur);

				headerReader->seek(0x10, std::ios::cur);
			}
			
			headerReader->seek(0x38, std::ios::cur);

			uint32_t unkStructCount = headerReader->readUInt32();
			for (uint32_t i = 0; i < unkStructCount; i++)
			{
				uint32_t unkIntCount = headerReader->readUInt32();
				
				// Cannot make any assumptions about this data

				// Usually below UINT32_MAX (obviously, if >0)
				headerReader->seek(4 * unkIntCount, std::ios::cur);

				// If 0, these are usually all below UINT32_MAX
				// If >0, usually all UINT32_MAX
				headerReader->seek(16, std::ios::cur);
			}

			headerReader->seek(12, std::ios::cur);

			uint32_t physXMeshCount = headerReader->readUInt32();
			for (uint32_t x = 0; x < physXMeshCount; x++)
			{
				uint32_t meshTypeEnum = headerReader->readUInt32();
				if (meshTypeEnum == 1)
					headerReader->seek(0x4C, std::ios_base::cur);
				else if (meshTypeEnum == 7)
					headerReader->seek(0x60, std::ios_base::cur);
				else
					headerReader->seek(0x50, std::ios_base::cur);

				// Vector4
				headerReader->seek(0x10, std::ios_base::cur);
				// Vector4
				headerReader->seek(0x10, std::ios_base::cur);

				if (meshTypeEnum != 0 && meshTypeEnum != 2 && meshTypeEnum != 3)
					headerReader->seek(0x4, std::ios_base::cur);

				// Removed number check
				headerReader->readInt32();

				headerReader->readUInt32();
				headerReader->seek(0x8, std::ios_base::cur);
			}

			uint32_t errorCount = headerReader->readUInt32();
			for (uint32_t x = 0; x < errorCount; x++)
			{
				uint32_t errorCountStrLen = headerReader->readUInt32();
				headerReader->seek(errorCountStrLen, std::ios_base::cur);
			}

			if (headerReader->tell() != headerReader->getLength())
				throw unknown_format_error("Did not reach end of file");
		}

		void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) override
		{
			uint32_t strLen = headerReader->readUInt32();
			headerReader->seek(strLen, std::ios::cur);
			
			uint32_t strLen2 = headerReader->readUInt32();
			headerReader->seek(strLen2, std::ios::cur);

			headerReader->seek(0x20, std::ios::cur);

			outHeader.faceCount = (uint32_t)headerReader->readUInt64();

			uint32_t meshInfoCount = headerReader->readUInt32();
			outHeader.meshInfos.resize(meshInfoCount);
			for (uint32_t x = 0; x < meshInfoCount; x++)
			{
				MeshInfoExternal& curMeshInfo = outHeader.meshInfos[x];

				headerReader->readFloatArray(&curMeshInfo.vector1.x, 4);
				headerReader->readFloatArray(&curMeshInfo.vector2.x, 4);

				uint32_t meshNameLen = headerReader->readUInt32();
				curMeshInfo.name = headerReader->readAsciiString(meshNameLen);

				headerReader->readUInt32Array(&curMeshInfo.faceLODOffsets[0], 5);
				headerReader->readUInt32Array(&curMeshInfo.faceLODCounts[0], 5);

				headerReader->seek(0x20, std::ios::cur);

				uint32_t nameLen = headerReader->readUInt32();
				headerReader->seek(nameLen, std::ios::cur);

				headerReader->seek(0x10, std::ios::cur);
			}

			outHeader.vertexCount = headerReader->readUInt32();

			headerReader->seek(0x10, std::ios::cur);

			outHeader.bodySkipLen1 = headerReader->readUInt32();

			headerReader->seek(0x20, std::ios::cur);

			uint32_t unkStructCount = headerReader->readUInt32();
			for (uint32_t x = 0; x < unkStructCount; x++)
			{
				uint32_t arrCount = headerReader->readUInt32();
				headerReader->seek(4 * arrCount, std::ios::cur);
				headerReader->seek(4 * 4, std::ios::cur);
			}

			headerReader->seek(12, std::ios::cur);

			uint32_t physXMeshCount = headerReader->readUInt32();
			outHeader.physXMeshes.resize(physXMeshCount);
			for (uint32_t x = 0; x < physXMeshCount; x++)
			{
				PhysXMesh& curPhys = outHeader.physXMeshes[x];

				curPhys.typeEnum = headerReader->readUInt32();
				if (curPhys.typeEnum == 1)
					headerReader->seek(0x4C, std::ios_base::cur);
				else if (curPhys.typeEnum == 7)
					headerReader->seek(0x60, std::ios_base::cur);
				else
					headerReader->seek(0x50, std::ios_base::cur);

				headerReader->seek(0x20, std::ios_base::cur);

				if (curPhys.typeEnum != 0 && curPhys.typeEnum != 2 && curPhys.typeEnum != 3)
					headerReader->seek(0x4, std::ios_base::cur);

				headerReader->seek(0x4, std::ios_base::cur);

				curPhys.dataLength = headerReader->readUInt32();
				headerReader->seek(0x8, std::ios_base::cur);
			}
		}

		void readBody(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody) override
		{
			for (const auto& x : extHeader.physXMeshes)
				bodyReader->seek(x.dataLength, std::ios_base::cur);

			bodyReader->seek(0x40 * extHeader.bodySkipLen1, std::ios::cur);

			outBody.positions.resize(extHeader.vertexCount);
			outBody.UV1.resize(extHeader.vertexCount);
			outBody.UV2.resize(extHeader.vertexCount);
			outBody.colors.resize(extHeader.vertexCount);
			for (uint32_t x = 0; x < extHeader.vertexCount; x++)
			{
				outBody.positions[x][0] = bodyReader->readHalf() - 0.5F;
				outBody.positions[x][1] = bodyReader->readHalf() - 0.5F;
				outBody.positions[x][2] = bodyReader->readHalf() - 0.5F;
				
				bodyReader->seek(10, std::ios::cur);
			}

			outBody.indices.resize(extHeader.faceCount);
			bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);
		}

		void readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader) override
		{

		}
	};
}
