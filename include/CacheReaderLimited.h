#pragma once

#include "BinaryReaderBuffered.h"
#include "PackageDir.h"

namespace WarframeExporter
{
	class CachePairLimited
	{
		const PackageReader::CachePair* m_cachePair;

	public:
		CachePairLimited(const PackageReader::CachePair* pair);

		const Entries::FileNode* getFileEntry(const std::string lotusPath) const;
		const Entries::DirNode* getDirEntry(const std::string lotusPath) const;
		uint8_t* getDataAndDecompress(const Entries::FileNode* entry) const;
	};

	class PackageLimited
	{
		PackageReader::Package* m_pkg;

	public:
		PackageLimited(PackageReader::Package* pkg);

		const CachePairLimited operator[](PackageReader::PackageTrioType trioType);
	};

	class PackageDirLimited
	{
		PackageReader::PackageDir* m_pkgDir;

	public:
		PackageDirLimited(PackageReader::PackageDir* pkg);

		PackageLimited operator[](std::string packageName);

		BinaryReaderBuffered* getFileReader(const std::string& packageName, PackageReader::PackageTrioType type, const std::string& internalPath);
	};
}