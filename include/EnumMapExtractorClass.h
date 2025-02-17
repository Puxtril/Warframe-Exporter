#pragma once

#include "Extractor.h"

namespace WarframeExporter
{
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
