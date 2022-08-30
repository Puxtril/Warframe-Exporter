#include "CacheReaderLimited.h"

using namespace WarframeExporter;

CachePairLimited::CachePairLimited(const PackageReader::CachePair* pair)
	: m_cachePair(pair)
{
}

const Entries::FileNode*
CachePairLimited::getFileEntry(const std::string lotusPath) const
{
	return m_cachePair->getFileEntry(lotusPath);
}

const Entries::DirNode*
CachePairLimited::getDirEntry(const std::string lotusPath) const
{
	return m_cachePair->getDirEntry(lotusPath);
}

uint8_t*
CachePairLimited::getDataAndDecompress(const Entries::FileNode* entry) const
{
	return m_cachePair->getDataAndDecompress(entry);
}

PackageLimited::PackageLimited(PackageReader::Package* pkg)
	: m_pkg(pkg)
{
}

const CachePairLimited
PackageLimited::operator[](PackageReader::PackageTrioType trioType)
{
	return CachePairLimited((*m_pkg)[trioType]);
}

PackageDirLimited::PackageDirLimited(PackageReader::PackageDir* pkg)
	: m_pkgDir(pkg)
{
}

PackageLimited
PackageDirLimited::operator[](std::string packageName)
{
	return PackageLimited(&(*m_pkgDir)[packageName]);
}

BinaryReaderBuffered*
PackageDirLimited::getFileReader(const std::string& packageName, PackageReader::PackageTrioType type, const std::string& internalPath)
{
	const PackageReader::CachePair* pkg = (*m_pkgDir)[packageName][type];
	if (pkg != nullptr)
	{
		try {
			const Entries::FileNode* fileNode = pkg->getFileEntry(internalPath);
			BinaryReaderBuffered* reader = new BinaryReaderBuffered(pkg->getDataAndDecompress(fileNode), fileNode->getLen());
			return reader;
		}
		catch (std::filesystem::filesystem_error&) {
			return nullptr;
		}
	}
	return nullptr;
}