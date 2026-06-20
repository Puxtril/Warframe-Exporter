#include "level/LevelConverter.h"

using namespace WarframeExporter::Level;

void
LevelConverter::convertToInternal(LevelHeaderExternal& extHeader, LevelBodyExternal& extBody, const std::string& internalLevelPath, LevelInternal& intBody)
{
	intBody.objs.resize(extHeader.levelObjs.size());
	auto parser = LotusLib::EENotationParser();
	const std::filesystem::path internalLevelPathObj(internalLevelPath);
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
		fixInternalPath(internalLevelPathObj, intObj.meshPath);
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
LevelConverter::splitAttributes(nlohmann::json::object_t& json, LevelObjectInternal& intObj)
{
	intObj.scale = 1.0F;

	for (auto& element : json)
	{
		if (element.first == "Mesh")
			intObj.meshPath = element.second.get<std::string>();
		else if (element.first == "Material")
			intObj.materials = {element.second.get<std::string>()};
		else if (element.first == "OverrideMaterial")
			intObj.materials = element.second.get<std::vector<std::string>>();
		else if (element.first == "MeshScale")
			intObj.scale = element.second.get<float>();
		else if (element.first == "MaterialParams")
			json["MaterialParamsPretty"] = getPrettyMaterialParams(element.second);
	}
}

void
LevelConverter::fixInternalPath(const std::filesystem::path& internalLevelPath, std::string& outPath)
{
	// Already absolute path
	if (outPath.size() < 5 || outPath[0] == '/')
		return;
	
	outPath = internalLevelPath.parent_path().string() + "/" + internalLevelPath.stem().string() + "/" + outPath;
}

std::vector<std::string>
LevelConverter::getPrettyMaterialParams(nlohmann::json inputJson)
{
	std::vector<std::string> params;

	auto materialParamsRaw = inputJson.get<std::vector<std::unordered_map<std::string, nlohmann::json>>>();
	for (size_t i = 0; i < materialParamsRaw.size(); i++)
	{
		std::string name = materialParamsRaw[i]["name"].get<std::string>();
		std::string valueType = materialParamsRaw[i]["type"].get<std::string>();
		nlohmann::json valueRaw = materialParamsRaw[i]["value"];

		if (valueType == "Float")
		{
			params.push_back(name + "=" + std::to_string(valueRaw.get<float>()));
		}
		else if (valueType == "Color")
		{
			std::array<float, 4> col = valueRaw.get<std::array<float, 4>>();
			params.push_back(name + "=[" + std::to_string(col[0]) + "," + std::to_string(col[1]) + "," + std::to_string(col[2]) + "," + std::to_string(col[3]) + "]");
		}
		else if (valueType == "Float4")
		{
			params.push_back(name + "=[" + std::to_string(materialParamsRaw[i]["value"].get<float>()) + "," + std::to_string(materialParamsRaw[i]["value1"].get<float>()) + "," + std::to_string(materialParamsRaw[i]["value2"].get<float>()) + "," + std::to_string(materialParamsRaw[i]["value3"].get<float>()) + "]");
		}
		else
		{
			throw unknown_format_error("Unknown MaterialParam Type: " + valueType);
		}
	}

	return params;
}