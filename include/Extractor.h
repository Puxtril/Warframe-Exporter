#pragma once

//#include "EnumMapExtractorValue.h"
#include "BinaryReaderBuffered.h"
#include "CommonHeader.h"
#include "ExporterLogger.h"
#include "LotusLib.h"
#include "ExtractOptions.h"

#include <string>
#include <vector>

namespace WarframeExporter
{
	enum class ExtractorType
	{
		Model = 1,
		Texture = 2,
		Material = 4,
		VERTEX_COLOR = 8,
		Level = 32,
		Audio = 64,
		Shader = 128,
		Landscape = 256,
		LevelStatic = 512,
	};

	// This is how Extractors register with specific Games and Package Categories.
	// `EnumMapExtractor` will statically check for double-registers.
    class EnumMapExtractorValue
	{
	public:
		virtual std::vector<std::tuple<LotusLib::Game, LotusLib::PackageCategory, int>> getEnumMapKeys() const = 0;
	};

	// Extractors wishing to register must inherit from this class
	class Extractor : public EnumMapExtractorValue
	{
	protected:
		Logger& m_logger;

		Extractor() : m_logger(Logger::getInstance()) {}
	
	public:
		virtual const std::string& getFriendlyName() const = 0;
		virtual const std::string& getOutputExtension(const LotusLib::CommonHeader& commonHeader, BinaryReader::BinaryReaderBuffered* hReader, WarframeExporter::ExtractOptions options) const = 0;
		// If true, `outputPath` in `extract` will be a folder.
		// Otherwise, will be a file with the correct extension
		virtual bool isMultiExport() const = 0;
		virtual ExtractorType getExtractorType() const = 0;
		virtual void extract(LotusLib::FileEntry& fileEntry, LotusLib::PackagesReader& pkgs, const std::filesystem::path& outputPath, ExtractOptions options) = 0;
	};

	// This class should only be instanced once, statically.
	// It registers extractors to a Game, Package Category, and a file type enumeration.
	// The file type enumeration is found within the Common Header, and is unique to 
	//   the game and Package Category.
	//
	// This class exists for quick lookups when processing files.
	//
	// An extra benefit that organically grew from design - since extractors register
	//   with specific Package Categories, one can derrive which Package categories must
	//   be iterated over to find files for a given extractor.
	// Ex. If you wanted to extract models from Warframe, the model extractor only registers
	//   to the Misc Package Category. So, you only need to look at those packages,
	//   and can avoid iterating over other packages like Texture and AnimRetarget.
	class EnumMapExtractor
	{
		std::map<LotusLib::Game, std::map<LotusLib::PackageCategory, std::map<int, Extractor*>>> m_enumMap;
		std::map<LotusLib::Game, std::map<ExtractorType, LotusLib::PackageCategory>> m_registeredPkgCategories;
		std::vector<Extractor*> m_extractors;
	
	public:
		EnumMapExtractor& registerClass(Extractor* cls);
		Extractor* at(LotusLib::Game game, LotusLib::PackageCategory pkgCat, int fileType) const;
		LotusLib::PackageCategory getPkgCategories(LotusLib::Game game, ExtractorType extractorType) const;
		LotusLib::PackageCategory getPkgCategories(LotusLib::Game game) const;
		const std::vector<Extractor*> getRegisteredExtractors() const;
	};
}
