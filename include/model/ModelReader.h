#pragma once

#include "BinaryReaderBuffered.h"
#include "model/ModelStructs.hpp"
#include "CommonHeader.h"
#include "EnumMapGame.h"
#include "model/ModelTypes.h"

namespace WarframeExporter::Model
{
	class ModelReader : public EnumMapGameValue
	{
	protected:
		ModelReader() = default;

	public:
		virtual ScaleType ensmalleningScale() const = 0;

		virtual void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) = 0;
		virtual void readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody) = 0;

	protected:
		void readWeightedBones(BinaryReader::BinaryReaderBuffered* reader, std::vector<std::string>& outWeightedBones);
		void readBoneTree(BinaryReader::BinaryReaderBuffered* reader, std::vector<BoneTreeNodeExternal>& outBoneTree);
		void readBoneMaps(BinaryReader::BinaryReaderBuffered* reader, std::vector<std::vector<uint32_t>>& outBoneMaps);
		void readMeshInfos(BinaryReader::BinaryReaderBuffered* reader, std::vector<MeshInfoExternal>& outMeshInfos);
		void readMaterialPaths(BinaryReader::BinaryReaderBuffered* reader, std::vector<std::string>& outMaterialpaths);
		void readPhysxMeshes(BinaryReader::BinaryReaderBuffered* reader, std::vector<PhysXMesh>& outPhysxMeshes);
		void readErrors(BinaryReader::BinaryReaderBuffered* reader, std::vector<std::string>& outErrors);

		void skipUnk16Array(BinaryReader::BinaryReaderBuffered* reader);
		void skipUnk64Array(BinaryReader::BinaryReaderBuffered* reader);

		void skipUnknownVector(BinaryReader::BinaryReaderBuffered* reader);
		uint32_t skipUnknownStructs(BinaryReader::BinaryReaderBuffered* reader);
		void skipMorphs(BinaryReader::BinaryReaderBuffered* reader);
		uint32_t skipMorphStructsAndFindSkip(BinaryReader::BinaryReaderBuffered* reader);
		void skipPhysicsStruct(BinaryReader::BinaryReaderBuffered* reader);

		bool canContinueReading(BinaryReader::BinaryReaderBuffered* reader, int vertexIndexCount);
	};
}