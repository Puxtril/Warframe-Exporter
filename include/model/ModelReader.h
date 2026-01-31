#pragma once

#include "BinaryReader/Buffered.h"
#include "model/ModelStructs.hpp"
#include "LotusLib/CommonHeader.h"
#include "EnumMapGame.h"
#include "ExporterExceptions.h"
#include "model/ModelTypes.h"

#include <algorithm>

namespace WarframeExporter::Model
{
	class ModelReader : public EnumMapGameValue
	{
	protected:
		ModelReader() = default;

	public:
		virtual ScaleType ensmalleningScale() const = 0;

		virtual void readHeader(BinaryReader::Buffered* headerReader, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) = 0;
		virtual void readBody(const ModelHeaderExternal& extHeader, BinaryReader::Buffered* bodyReaderB, BinaryReader::Buffered* bodyReaderF, ModelBodyExternal& outBody) = 0;

	protected:
		void readWeightedBones(BinaryReader::Buffered* reader, std::vector<std::string>& outWeightedBones);
		void readBoneTree(BinaryReader::Buffered* reader, std::vector<BoneTreeNodeExternal>& outBoneTree);
		void readBoneMaps(BinaryReader::Buffered* reader, std::vector<std::vector<uint32_t>>& outBoneMaps);
		void readMeshInfos(BinaryReader::Buffered* reader, std::vector<MeshInfoExternal>& outMeshInfos);
		void readMaterialPaths(BinaryReader::Buffered* reader, std::vector<std::string>& outMaterialpaths);
		void readPhysxMeshes(BinaryReader::Buffered* reader, std::vector<PhysXMesh>& outPhysxMeshes);
		void readErrors(BinaryReader::Buffered* reader, std::vector<std::string>& outErrors);

		void skipUnk16Array(BinaryReader::Buffered* reader);
		void skipUnk64Array(BinaryReader::Buffered* reader);

		void skipUnknownVector(BinaryReader::Buffered* reader);
		uint32_t skipUnknownStructs(BinaryReader::Buffered* reader);
		void skipMorphs(BinaryReader::Buffered* reader);
		uint32_t skipMorphStructsAndFindSkip(BinaryReader::Buffered* reader, std::vector<MeshInfoExternal>& outMeshInfos);
		void skipPhysicsStruct(BinaryReader::Buffered* reader);
		void skipPhysicsStruct2(BinaryReader::Buffered* reader);

		bool canContinueReading(BinaryReader::Buffered* reader, int vertexIndexCount);
	};
}