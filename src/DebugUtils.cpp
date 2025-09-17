#include "DebugUtils.h"

using namespace WarframeExporter;

void
DebugUtils::printEnumCounts(LotusLib::PackageReader& pkg, const LotusLib::LotusPath& internalPath)
{
	std::map<uint32_t, int> enumCounts;
	std::map<uint32_t, std::vector<std::string>> enumExamples;

	Logger::getInstance().info("Collecting Common Header Format stats");
	for (auto iter = pkg.getIter(internalPath); iter != pkg.getIter(); iter++)
	{
		try
		{
			LotusLib::FileEntry curFile = pkg.getFile(*iter, LotusLib::FileEntryReaderFlags::READ_COMMON_HEADER);

			enumCounts[curFile.commonHeader.type]++;
			
			// For the first 10 files, add directly into examples
			if (enumExamples[curFile.commonHeader.type].size() < 10)
				enumExamples[curFile.commonHeader.type].push_back(curFile.internalPath.string());
			
			// Every 10 new files, replace an existing example
			else if (enumCounts[curFile.commonHeader.type] % 10 == 0)
			{
				int newIndex = enumCounts[curFile.commonHeader.type] % 100 / 10;
				enumExamples[curFile.commonHeader.type][newIndex] = curFile.internalPath.string();
			}
		}
		catch (LotusLib::DecompressionException&)
		{
			Logger::getInstance().warn("Decompress error: " + internalPath.string());
			continue;
		}
		catch (LotusLib::LotusException& ex)
		{
			Logger::getInstance().error(ex.what());
			continue;
		}
		catch (LimitException& ex)
		{
			Logger::getInstance().warn(std::string(ex.what()) + internalPath.string());
			continue;
		}
		catch (std::exception& ex)
		{
			Logger::getInstance().error(std::string(ex.what()) + ": " + internalPath.string());
			continue;
		}
	}

	for (auto& x : enumCounts)
	{
		std::cout << x.first << ": " << x.second << std::endl;
		for (auto& str : enumExamples[x.first])
			std::cout << "  " << str << std::endl;
	}
}

void
DebugUtils::writeAllDebugs(LotusLib::PackageReader& pkg, LotusLib::FileEntry& fileEntry, const std::filesystem::path& baseOutputPath)
{
	std::filesystem::path debugPath = baseOutputPath / "Debug" / fileEntry.internalPath.relative_path();
	if (!std::filesystem::exists(debugPath.parent_path()))
		std::filesystem::create_directories(debugPath.parent_path());

	if (fileEntry.headerData.getLength() > 0)
	{
		debugPath.replace_filename(fileEntry.internalPath.filename().string() + "_H");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)fileEntry.headerData.getPtr().data(), fileEntry.headerData.getLength());
		out.close();
	}

	if (fileEntry.bData.getLength() > 0)
	{
		debugPath.replace_filename(fileEntry.internalPath.filename().string() + "_B");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)fileEntry.bData.getPtr().data(), fileEntry.bData.getLength());
		out.close();
	}

	if (fileEntry.fData.getLength() > 0)
	{
		debugPath.replace_filename(fileEntry.internalPath.filename().string() + "_F");
		std::ofstream out;
		out.open(debugPath, std::ios::binary | std::ios::out | std::ofstream::trunc);
		out.write((char*)fileEntry.fData.getPtr().data(), fileEntry.fData.getLength());
		out.close();
	}
}
