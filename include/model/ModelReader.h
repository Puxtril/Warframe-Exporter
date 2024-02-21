#pragma once

#include "BinaryReaderBuffered.h"
#include "Ensmallening.hpp"
#include "model/ModelStructs.hpp"
#include "CommonHeader.h"
#include "EnumMapValue.h"

namespace WarframeExporter::Model
{
	enum class ModelType
	{
		MODEL_STATIC_86 = 86,
		MODEL_STATIC_96 = 96,
		MODEL_LEVEL_99 = 99,
		MODEL_DCM_101 = 101,
		MODEL_HLOD_OR_DCM_102 = 102, // Thanks, DE!
		MODEL_LEVEL_103 = 103,
		MODEL_TERRAIN_159 = 159,
		MODEL_TERRAIN_160 = 160,
		MODEL_RIGGED_269 = 269,
		MODEL_RIGGED_272 = 272,
		MODEL_PACKED_289 = 289,
		MODEL_PACKED_290 = 290
	};

	enum class ScaleType
	{
		XYZ,
		XZ,
		NONE
	};

	class ModelReader : public EnumMapValue
	{
	protected:
		ModelReader() = default;

	public:
		virtual ScaleType ensmalleningScale() const = 0;

		virtual void readHeaderDebug(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header) = 0;
		virtual void readHeader(BinaryReader::BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const LotusLib::CommonHeader& header, ModelHeaderExternal& outHeader) = 0;
		virtual void readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader) = 0;
		virtual void readBody(const ModelHeaderExternal& extHeader, BinaryReader::BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody) = 0;
	};
}