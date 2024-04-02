#pragma once

#include "BinaryReaderBuffered.h"
#include "Ensmallening.hpp"
#include "model/ModelStructs.hpp"
#include "CommonHeader.h"
#include "EnumMapValue.h"
#include "model/ModelTypes.h"

namespace WarframeExporter::Model
{
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