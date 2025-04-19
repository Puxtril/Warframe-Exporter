#include "level/LevelConverter.h"

using namespace WarframeExporter::Level;

void
LevelConverter::convertToInternal(LevelHeaderExternal& extHeader, LevelBodyExternal& extBody, const LotusLib::LotusPath& internalLevelPath, LevelInternal& intBody)
{
	intBody.objs.resize(extHeader.levelObjs.size());
	auto parser = LotusLib::LotusNotationParser();
	for (size_t x = 0; x < intBody.objs.size(); x++)
	{
		LevelObjectHeaderExternal& extObj = extHeader.levelObjs[x];
		LevelObjectInternal& intObj = intBody.objs[x];
		intObj.attributes = parser.parse(extBody.attributes[x].data(), extBody.attributes[x].size());
		intObj.attributes["Type"] = extHeader.levelObjs[x].objTypePath;
		
		intObj.objTypePath = std::move(extObj.objTypePath);
		intObj.objName = std::move(extObj.objName);
		intObj.pos = std::move(extObj.pos);
		intObj.rot = std::move(extObj.rot);

		splitAttributes(intObj.attributes, intBody.objs[x]);
		fixInternalPath(internalLevelPath, intObj.meshPath);
	}
}

void
LevelConverter::replaceOverrideMaterials(const std::vector<std::string>& materialNames, Model::ModelHeaderInternal& modelHeader)
{
	size_t replaceCount = std::min(materialNames.size(), modelHeader.meshInfos.size());

	for (size_t x = 0; x < replaceCount; x++)
		if (materialNames[x].size() > 2)
			modelHeader.meshInfos[x].matName = materialNames[x];
}

void
LevelConverter::splitAttributes(nlohmann::json& json, LevelObjectInternal& intObj)
{
	intObj.scale = 1.0F;

	for (auto& element : json.items())
	{
		if (element.key() == "Mesh")
			intObj.meshPath = element.value().get<std::string>();
		else if (element.key() == "OverrideMaterial")
			intObj.materials = element.value().get<std::vector<std::string>>();
		else if (element.key() == "MeshScale")
			intObj.scale = element.value().get<float>();
	}
}

void
LevelConverter::fixInternalPath(const LotusLib::LotusPath& internalLevelPath, std::string& outPath)
{
	// Already absolute path
	if (outPath.size() < 5 || outPath[0] == '/')
		return;
	
	outPath = internalLevelPath.parent_path().string() + "/" + internalLevelPath.stem().string() + "/" + outPath;
}

void
LevelConverter::convertLandscapeToInternal(const LevelExternal& levelExternal, LevelInternal& levelInternal)
{
	if (levelExternal.landscapeIndex == -1)
	{
		Logger::getInstance().debug("Landscape not found");
		return;
	}

	const LevelObjectHeaderExternal& objHeader = levelExternal.header.levelObjs[levelExternal.landscapeIndex];
	const std::vector<char>& attributesRaw = levelExternal.body.attributes[levelExternal.landscapeIndex];

	auto parser = LotusLib::LotusNotationParser();
	nlohmann::json attrs = parser.parse(attributesRaw.data(), attributesRaw.size());

	levelInternal.landscape.pos = objHeader.pos;
	levelInternal.landscape.landscapePath = attrs["DataRes"].get<std::string>();
	
	if (attrs.contains("Material"))
		levelInternal.landscape.attributes["Material"] = attrs["Material"].get<std::string>();
	if (attrs.contains("MaterialLOD"))
		levelInternal.landscape.attributes["MaterialLOD"] = attrs["MaterialLOD"].get<std::string>();
	if (attrs.contains("OverrideMaterial"))
		levelInternal.landscape.attributes["OverrideMaterial"] = attrs["OverrideMaterial"].get<std::vector<std::string>>();
}