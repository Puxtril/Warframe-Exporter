#pragma once

#include "BinaryReaderExceptions.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "model/ModelReader.h"
#include "ExporterExceptions.h"

#include <cassert>
#include <cstring>
#include <iomanip>
#include <string_view>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

namespace WarframeExporter::Model
{
	class ModelReader102 : public ModelReader
	{
		ModelReader102() = default;

	public:
		inline static ModelReader102* getInstance()
		{
			static ModelReader102* instance = new ModelReader102();
			return instance;
		}

		inline std::vector<int> getEnumMapKeys() const override
		{
			std::vector<int> extTypes = { (int)ModelType::MODEL_HLOD_OR_DCM_102 };
			return extTypes;
		}

		inline ScaleType ensmalleningScale() const override
		{
			return ScaleType::XYZ;
		}

		// Hahahaha
		// This sucks, man
		// True for DCM, False for HLOD
		inline bool isDCM(const LotusLib::CommonHeader& header)
		{
			if (header.paths.size() != 1)
				return false;

			const std::string& filePath = header.paths[0];

			constexpr std::string_view dcmExt1 = "_dcm.fbx";
			constexpr std::string_view dcmExt2 = "_dcm";

			std::string_view ext1 = std::string_view(filePath.data() + (filePath.length() - 8), 8);
			std::string_view ext2 = std::string_view(filePath.data() + (filePath.length() - 4), 4);

			if (dcmExt1 == ext1 || dcmExt2 == ext2)
				return true;
			
			return false;
		}

		inline void skipPhysicsPath(BinaryReaderBuffered* headerReader) const
		{
			uint32_t structType = headerReader->readUInt32();
			uint32_t subType = headerReader->readUInt32();

			// Perhaps these 2 numbers are flags?
			// Doesn't matter... debugging this took long enough. It works.
			if (subType & 8)
			{
				headerReader->seek(2, std::ios::cur);
			}

			uint32_t pathLen = headerReader->readUInt32();
			headerReader->seek(pathLen, std::ios::cur);
		}

		void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) override
		{
			headerReader->seek(0x1C, std::ios_base::cur);
			delete[] headerReader->readUInt32Array(5); // Possible LOD Data
			
			this->skipPhysicsPath(headerReader);
		
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

			headerReader->seek(0x18, std::ios_base::cur);

			uint32_t arrSkip2 = headerReader->readUInt32();
			headerReader->seek(8 * arrSkip2, std::ios_base::cur);

			headerReader->seek(0x51, std::ios_base::cur);

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

			uint32_t someSkip = headerReader->readUInt32();
			if (someSkip == 2)
				headerReader->seek(4, std::ios::cur);
			else if (someSkip == 3)
				headerReader->seek(6, std::ios::cur);

			bool isDCM = this->isDCM(header);

			if (isDCM)
			{
				headerReader->seek(0x5, std::ios::cur);
			}
			else
			{
				headerReader->seek(0x4, std::ios::cur);
			}

			this->skipPhysicsPath(headerReader);

			uint32_t physXMeshCount = headerReader->readUInt32();
			for (uint32_t x = 0; x < physXMeshCount; x++)
			{
				static const std::string meshTypeMsg = "PhysX Mesh Type";
				uint32_t meshTypeEnum = headerReader->readUInt32(0, 20, meshTypeMsg);

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
		}

		void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) override
		{
			headerReader->seek(0x30, std::ios_base::cur);

			this->skipPhysicsPath(headerReader);

			headerReader->seek(0x4E, std::ios_base::cur);

			outHeader.vertexCount = headerReader->readUInt32();
			outHeader.faceCount = headerReader->readUInt32();
			outHeader.morphCount = headerReader->readUInt32();
			outHeader.boneCount = headerReader->readUInt32();

			headerReader->seek(0x18, std::ios_base::cur);

			uint32_t arrSkip2 = headerReader->readUInt32();
			headerReader->seek(8 * arrSkip2, std::ios_base::cur);

			headerReader->seek(0x51, std::ios_base::cur);

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

			uint32_t someSkip = headerReader->readUInt32();
			if (someSkip == 2)
				headerReader->seek(4, std::ios::cur);
			else if (someSkip == 3)
				headerReader->seek(6, std::ios::cur);

			bool isDCM = this->isDCM(header);

			if (isDCM)
			{
				headerReader->seek(0x5, std::ios::cur);
			}
			else
			{
				headerReader->seek(0x4, std::ios::cur);
			}

			this->skipPhysicsPath(headerReader);

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
		}

		void readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader) override
		{
			for (const auto& x : extHeader.physXMeshes)
				bodyReader->seek(x.dataLength, std::ios_base::cur);

			bodyReader->seek(64 * extHeader.boneCount, std::ios_base::cur);

			bodyReader->seek(0x2, std::ios_base::cur);

			for (uint32_t x = 0; x < extHeader.vertexCount; x++)
			{
				bodyReader->seek(24, std::ios_base::cur);
			}

			size_t remainingBytes = bodyReader->getLength() - bodyReader->tell();
			size_t indicesSize = static_cast<size_t>(extHeader.faceCount) * 2;
			
			// DCM Model
			if (remainingBytes > indicesSize)
			{
				bodyReader->seek(extHeader.vertexCount * 8, std::ios_base::cur);
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

			bodyReader->seek(0x2, std::ios_base::cur);

			outBody.positions.resize(extHeader.vertexCount);
			outBody.UV1.resize(extHeader.vertexCount);
			outBody.UV2.resize(extHeader.vertexCount);
			for (uint32_t x = 0; x < extHeader.vertexCount; x++)
			{
				outBody.positions[x][0] = bodyReader->readInt16() / 32767.0F;
				outBody.positions[x][1] = bodyReader->readInt16() / 32767.0F;
				outBody.positions[x][2] = bodyReader->readInt16() / 32767.0F;

				// /shrug
				bodyReader->seek(10, std::ios_base::cur);

				outBody.UV1[x][0] = bodyReader->readHalf();
				outBody.UV1[x][1] = bodyReader->readHalf();

				outBody.UV2[x][0] = bodyReader->readHalf();
				outBody.UV2[x][1] = bodyReader->readHalf();
			}

			size_t remainingBytes = bodyReader->getLength() - bodyReader->tell();
			size_t indicesSize = static_cast<size_t>(extHeader.faceCount) * 2;
			
			// DCM Model
			if (remainingBytes > indicesSize)
			{
				bodyReader->seek(extHeader.vertexCount * 8, std::ios_base::cur);
			}

			outBody.indices.resize(extHeader.faceCount);
			bodyReader->readUInt16Array(outBody.indices.data(), extHeader.faceCount);
		}
};
};
