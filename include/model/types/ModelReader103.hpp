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
	class ModelReader103 : public ModelReader
	{
		ModelReader103() = default;

	public:
		inline static ModelReader103* getInstance()
		{
			static ModelReader103* instance = new ModelReader103();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)ModelType::MODEL_LEVEL_103 };
			return extTypes;
		}

		inline ScaleType ensmalleningScale() const override
		{
			return ScaleType::NONE;
		}

		void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) override
		{
			headerReader->seek(0x1C, std::ios_base::cur);
			delete headerReader->readUInt32Array(5); // Possible LOD Data
			headerReader->readUInt32(); // Not an array
			headerReader->readUInt32(); // Not an array

			// Missing pre-check, len is non-zero, path doesn't exist: /Lotus/Characters/Orokin/OrokinStatue/OrokinStatueDestroyedHead.fbx
			// Missing pre-check, len is non-zero, path exists:        /Lotus/Characters/Grineer/GrineerTwinQueens/GrineerQueenSceptreBrokenVesselB.fbx
			// Has pre-check,     len is zero,     path doesn't exist: /Lotus/Characters/NewWar/Tefillah/TefillahDamaged.fbx
			// Has pre-check,     len is non-zero, path exists:        /Lotus/Objects/SolarisUnited/ExteriorKit/SUExtPipe90Corner.fbx
			static const std::string physicsPath1 = "First physics path Check";
			uint32_t physicsPathCheck = headerReader->readUInt16(0, (int)UINT8_MAX + 1, physicsPath1);
			if (physicsPathCheck > 0)
			{
				uint32_t physPathLen;
				if (physicsPathCheck == 255)
				{
					physPathLen = headerReader->readUInt32();
				}
				else
				{
					physPathLen = physicsPathCheck;
					headerReader->seek(0x2, std::ios_base::cur);
				}
			
				// Because the path may not actually exist...
				if (physPathLen > 0)
				{
					uint16_t nullCheck = headerReader->readUInt16();
					if (nullCheck != 0)
						headerReader->seek(physPathLen - 2, std::ios_base::cur);
				}
			}
			else
				headerReader->seek(0x2, std::ios_base::cur);
		
			headerReader->seek(0x18, std::ios_base::cur);

			// 99% this is 1
			// 100: /Lotus/Characters/Tenno/Operator/Hair/HairD/HairD_sort.fbx
			// 50:  /Lotus/Characters/Tenno/Operator/Hair/HairO/HairO_sort.fbx
			static const std::string randomFloat = "Random float test";
			headerReader->readFloat(1.0F, 101.0F, CONV_ZERO | FAIL_SUBNORM, randomFloat);

			headerReader->seek(0x12, std::ios_base::cur);
		
			// Vector4
			static const std::string vec4Msg = "Vector4 pair";
			headerReader->seek(0xC, std::ios_base::cur);
			headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, vec4Msg);
			// Vector4
			headerReader->seek(0xC, std::ios_base::cur);
			headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, vec4Msg);

			uint32_t vertexCount = headerReader->readUInt32();

			static const std::string faceCountMsg = "Face count";
			headerReader->readUInt32(vertexCount, UINT32_MAX, faceCountMsg);

			headerReader->readUInt32(); // morph count
			headerReader->readUInt32(); // bone count

			headerReader->seek(0xC, std::ios_base::cur);

			headerReader->readUInt32();

			headerReader->seek(0x8, std::ios_base::cur);

			static const std::string always1IntMsg = "Always 1 integer";
			headerReader->readUInt32(1, 1, always1IntMsg);

			headerReader->seek(0x59, std::ios_base::cur);

			const static std::string meshInfoMsg = "Mesh Info Count";
			uint32_t minMeshInfo = vertexCount > 0 ? 1 : 0;
			uint32_t meshInfoCount = headerReader->readUInt32(minMeshInfo, 30, meshInfoMsg);

			for (uint32_t x = 0; x < meshInfoCount; x++)
			{
				static const std::string meshInfoVec4Msg = "MeshInfo Vector4 pair";
				headerReader->seek(0xC, std::ios_base::cur);
				headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, meshInfoVec4Msg);
				headerReader->seek(0xC, std::ios_base::cur);
				headerReader->readFloat(0.0F, 0.00001F, CONV_ZERO | FAIL_SUBNORM, meshInfoVec4Msg);

				uint32_t meshInfoNameLen = headerReader->readUInt32();
				headerReader->seek(meshInfoNameLen, std::ios_base::cur);

				auto meshInfoOffset = headerReader->tell();
				try {
					static const std::string LODOffsetMsg = "MeshInfo LOD Offset";
					uint32_t curFaceLODOffset = headerReader->readUInt32();
					curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetMsg);
					curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetMsg);
					curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetMsg);
					curFaceLODOffset = headerReader->readUInt32(curFaceLODOffset, UINT32_MAX, LODOffsetMsg);

					static const std::string LODFaceCountMsg = "MeshInfo LOD Face";
					uint32_t curFaceLODCount = headerReader->readUInt32();
					curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
					curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
					curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
					curFaceLODCount = headerReader->readUInt32(0, curFaceLODCount + 1, LODFaceCountMsg);
				}
				// Edge case where only 1 LOD exists
				catch (LimitException&)
				{
					headerReader->seek(meshInfoOffset, std::ios_base::beg);

					static const std::string LODOffsetMsg = "MeshInfo LOD Offset edge case";
					headerReader->readUInt32(0, UINT32_MAX, LODOffsetMsg);
					delete headerReader->readUInt32Array(4, 0, 1, LODOffsetMsg);

					static const std::string LODFaceCountMsg = "MeshInfo LOD Face count edge case";
					headerReader->readUInt32(0, UINT32_MAX, LODFaceCountMsg);
					delete headerReader->readUInt32Array(4, 0, 1, LODFaceCountMsg);
				}

				headerReader->seek(0x34, std::ios_base::cur);
			}
		
			uint32_t shortCount = headerReader->readUInt32();
			headerReader->seek(shortCount * 2U, std::ios_base::cur);
		
			uint32_t materialPathArrayLen = headerReader->readUInt32();
			for (uint32_t x = 0; x < materialPathArrayLen; x++)
			{
				uint32_t curMaterialPathLen = headerReader->readUInt32();
				headerReader->seek(curMaterialPathLen, std::ios::cur);
			}

			headerReader->seek(0x8, std::ios::cur);

			static const std::string physicsPath2 = "Second physics path";
			int32_t physicsPath2Check = headerReader->readUInt16(0, (int)UINT8_MAX + 1, physicsPath2);
			if (physicsPath2Check > 0)
			{
				uint32_t physPath2Len;
				if (physicsPath2Check == 255)
				{
					physPath2Len = headerReader->readUInt32();
				}
				else
				{
					physPath2Len = physicsPath2Check;
					headerReader->seek(0x2, std::ios_base::cur);
				}

				// Because the path may not actually exist...
				if (physPath2Len > 0)
				{
					uint16_t nullCheck = headerReader->readUInt16();
					if (nullCheck != 0)
						headerReader->seek(physPath2Len - 2, std::ios_base::cur);
				}
			}
			else
				headerReader->seek(0x2, std::ios_base::cur);

			uint32_t physXMeshCount = headerReader->readUInt32();
			for (uint32_t x = 0; x < physXMeshCount; x++)
			{
				uint32_t meshTypeEnum = headerReader->readUInt32();
				if (meshTypeEnum == 1)
					headerReader->seek(0x4C, std::ios_base::cur);
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
			headerReader->seek(0x38, std::ios_base::cur);

			uint32_t physicsPathCheck = headerReader->readUInt16();
			if (physicsPathCheck > 0)
			{
				uint32_t physPathLen;
				if (physicsPathCheck == 255)
				{
					physPathLen = headerReader->readUInt32();
				}
				else
				{
					physPathLen = physicsPathCheck;
					headerReader->seek(0x2, std::ios_base::cur);
				}

				// Because the path may not actually exist...
				if (physPathLen > 0)
				{
					uint16_t nullCheck = headerReader->readUInt16();
					if (nullCheck != 0)
						headerReader->seek(physPathLen - 2, std::ios_base::cur);
				}
			}
			else
				headerReader->seek(0x2, std::ios_base::cur);

			headerReader->seek(0x4E, std::ios_base::cur);

			outHeader.vertexCount = headerReader->readUInt32();
			outHeader.faceCount = headerReader->readUInt32();
			outHeader.morphCount = headerReader->readUInt32();
			outHeader.boneCount = headerReader->readUInt32();

			headerReader->seek(0x75, std::ios_base::cur);

			uint32_t meshInfoCount = headerReader->readUInt32();
			outHeader.meshInfos.resize(meshInfoCount);
			for (uint32_t x = 0; x < meshInfoCount; x++)
			{
				MeshInfoExternal& meshInfo = outHeader.meshInfos[x];

				headerReader->readFloatArray(&meshInfo.vector1.x, 4);
				headerReader->readFloatArray(&meshInfo.vector2.x, 4);

				uint32_t meshInfoNameLen = headerReader->readUInt32();
				meshInfo.name = headerReader->readAsciiString(meshInfoNameLen);

				headerReader->readUInt32Array(meshInfo.faceLODOffsets.data(), 5);
				headerReader->readUInt32Array(meshInfo.faceLODCounts.data(), 5);

				headerReader->seek(0x34, std::ios_base::cur);
			}

			uint32_t shortCount = headerReader->readUInt32();
			headerReader->seek(shortCount * 2U, std::ios_base::cur);

			uint32_t materialPathArrayLen = headerReader->readUInt32();
			for (uint32_t x = 0; x < materialPathArrayLen; x++)
			{
				uint32_t curMaterialPathLen = headerReader->readUInt32();
				outHeader.materialPaths.push_back(headerReader->readAsciiString(curMaterialPathLen));
			}

			headerReader->seek(0x8, std::ios::cur);

			static const std::string physicsPath2 = "Second physics path";
			int32_t physicsPath2Check = headerReader->readUInt16(0, (int)UINT8_MAX + 1, physicsPath2);
			if (physicsPath2Check > 0)
			{
				uint32_t physPath2Len;
				if (physicsPath2Check == 255)
				{
					physPath2Len = headerReader->readUInt32();
				}
				else
				{
					physPath2Len = physicsPath2Check;
					headerReader->seek(0x2, std::ios_base::cur);
				}

				// Because the path may not actually exist...
				if (physPath2Len > 0)
				{
					uint16_t nullCheck = headerReader->readUInt16();
					if (nullCheck != 0)
						headerReader->seek(physPath2Len - 2, std::ios_base::cur);
				}
			}
			else
				headerReader->seek(0x2, std::ios_base::cur);

			uint32_t physXMeshCount = headerReader->readUInt32();
			outHeader.physXMeshes.resize(physXMeshCount);
			for (uint32_t x = 0; x < physXMeshCount; x++)
			{
				PhysXMesh& curPhysXMesh = outHeader.physXMeshes[x];

				curPhysXMesh.typeEnum = headerReader->readUInt32();
				if (curPhysXMesh.typeEnum == 1)
					headerReader->seek(0x4C, std::ios_base::cur);
				else
					headerReader->seek(0x50, std::ios_base::cur);

				headerReader->readFloatArray(&curPhysXMesh.vector1.x, 4);
				headerReader->readFloatArray(&curPhysXMesh.vector2.x, 4);

				if (curPhysXMesh.typeEnum != 0 && curPhysXMesh.typeEnum != 2 && curPhysXMesh.typeEnum != 3)
					headerReader->seek(0x4, std::ios_base::cur);

				headerReader->seek(0x4, std::ios_base::cur);

				curPhysXMesh.dataLength = headerReader->readUInt32();
				headerReader->seek(0x8, std::ios_base::cur);
			}

			uint32_t errorCount = headerReader->readUInt32();
			for (uint32_t x = 0; x < errorCount; x++)
			{
				uint32_t errorCountStrLen = headerReader->readUInt32();
				outHeader.errorMsgs.push_back(headerReader->readAsciiString(errorCountStrLen));
			}

			if (headerReader->tell() != headerReader->getLength())
				throw unknown_format_error("Did not reach end of file");
		}

		void readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader) override
		{
			for (const auto& x : extHeader.physXMeshes)
				bodyReader->seek(x.dataLength, std::ios_base::cur);

			bodyReader->seek(64 * extHeader.boneCount, std::ios_base::cur);

			// Such a hack, but it works very well (>99% success for models)
			// Most of the PhysX mesh data is empty space
			while (isMorePhysX(bodyReader))
				bodyReader->seek(0x120, std::ios::cur);

			bodyReader->seek(0x2, std::ios_base::cur);

			for (uint32_t x = 0; x < extHeader.vertexCount; x++)
			{
				bodyReader->seek(0x20, std::ios_base::cur); // /shrug
			}

			static const std::string facesIndiciesMsg = "Face Indices";
			delete bodyReader->readUInt16Array(extHeader.faceCount, 0, extHeader.vertexCount + 1, facesIndiciesMsg);

			if (bodyReader->tell() != bodyReader->getLength())
				throw unknown_format_error("Did not reach end of file");
		}

		void readBody(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody) override
		{
			for (const auto& x : extHeader.physXMeshes)
				bodyReader->seek(x.dataLength, std::ios_base::cur);

			bodyReader->seek(64U * extHeader.boneCount, std::ios_base::cur);

			while (isMorePhysX(bodyReader))
				bodyReader->seek(0x120, std::ios::cur);

			bodyReader->seek(0x2, std::ios_base::cur);

			outBody.positions.resize(extHeader.vertexCount);
			outBody.UV1.resize(extHeader.vertexCount);
			outBody.UV2.resize(extHeader.vertexCount);
			outBody.colors.resize(extHeader.vertexCount);
			outBody.boneIndices.resize(extHeader.vertexCount);
			outBody.boneWeights.resize(extHeader.vertexCount);
			for (uint32_t x = 0; x < extHeader.vertexCount; x++)
			{
				outBody.positions[x][0] = bodyReader->readFloat();
				outBody.positions[x][1] = bodyReader->readFloat();
				outBody.positions[x][2] = bodyReader->readFloat();

				// /shrug
				bodyReader->seek(12, std::ios_base::cur);

				outBody.UV1[x][0] = bodyReader->readHalf();
				outBody.UV1[x][1] = bodyReader->readHalf();

				outBody.UV2[x][0] = bodyReader->readHalf();
				outBody.UV2[x][1] = bodyReader->readHalf();
			}

			outBody.indices.resize(extHeader.faceCount);
			bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);
		}
	
		// This is 100% a workaround
		bool isMorePhysX(BinaryReaderBuffered* bodyReader)
		{
			// Some models have no actual vertices, so this can hit EOF
			if (bodyReader->tell() + 0x120 > bodyReader->getLength())
				return false;

			// Some structs have data pretty far into the allocated 0x120 block size
			bodyReader->seek(0x100, std::ios::cur);
			uint64_t check1 = bodyReader->readUInt64();
			uint64_t check2 = bodyReader->readUInt64();
			bodyReader->seek(-0x110, std::ios::cur);

			return check1 == 0 && check2 == 0;
		}
};

	
}
