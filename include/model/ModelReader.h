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
		MODEL_STATIC_96 = 96,
		MODEL_RIGGED_269 = 269,
		MODEL_RIGGED_272 = 272
	};

	class ModelReader : public EnumMapValue
	{
	protected:
		ModelReader() = default;

	public:
		virtual void readHeaderDebug(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header) = 0;
		virtual void readHeader(BinaryReaderBuffered* headerReader, const Ensmallening& ensmalleningData, const CommonFileHeader& header, ModelHeaderExternal& outHeader) = 0;
		virtual void readBodyDebug(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader) = 0;
		virtual void readBody(const ModelHeaderExternal& extHeader, BinaryReaderBuffered* bodyReader, ModelBodyExternal& outBody) = 0;
	};
}