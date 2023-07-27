#include "level/LevelConverter.h"

using namespace WarframeExporter::Level;

void
LevelConverter::convertToInternal(LevelHeaderExternal& extHeader, LevelBodyExternal& extBody, const LotusLib::LotusPath& internalLevelPath, LevelInternal& intBody)
{
	intBody.objs.resize(extHeader.levelObjs.size());
	for (size_t x = 0; x < intBody.objs.size(); x++)
	{
		LevelObjectHeaderExternal& extObj = extHeader.levelObjs[x];
		LevelObjectInternal& intObj = intBody.objs[x];
		
		intObj.rawAttributes = std::move(extBody.attributes[x]);
		intObj.objTypePath = std::move(extObj.objTypePath);
		intObj.objName = std::move(extObj.objName);
		intObj.pos = std::move(extObj.pos);
		intObj.rot = std::move(extObj.rot);

		splitAttributes(intObj.rawAttributes, intBody.objs[x]);
		fixInternalPath(internalLevelPath, intObj.meshPath);
	}
}

void
LevelConverter::splitAttributes(const std::string& attrs, LevelObjectInternal& intObj)
{
	constexpr std::string_view meshKey = "Mesh=";
	std::string_view meshPath = "";
	findString(attrs, meshKey, meshPath);
	intObj.meshPath = std::string(meshPath);

	constexpr std::string_view materialOverrideKey = "OverrideMaterial={";
	if (!findStringArray(attrs, materialOverrideKey, intObj.materials))
		intObj.materials = {};

	constexpr std::string_view scaleKey = "MeshScale=";
	if (!findFloat(attrs, scaleKey, intObj.scale))
		intObj.scale = 1.0;

	constexpr std::string_view lightmap1Key = "LightMap=";
	std::string_view lightMapvalue;
	if (findString(attrs, lightmap1Key, lightMapvalue))
		intObj.attributes[lightmap1Key] = lightMapvalue;

	constexpr std::string_view lightmapCoordsKey = "LightMapCoords=";
	std::string_view lightMapCordsValue;
	if (findString(attrs, lightmapCoordsKey, lightMapCordsValue))
		intObj.attributes[lightmapCoordsKey] = lightMapCordsValue;

	constexpr std::string_view lightmapNormalizeKey = "LightMapNormalize=";
	std::string_view lightMapNormalizeValue;
	if (findString(attrs, lightmapNormalizeKey, lightMapNormalizeValue))
		intObj.attributes[lightmapNormalizeKey] = lightMapNormalizeValue;

	constexpr std::string_view materialSwapKey = "MaterialForSwap=";
	std::string_view materialSwapValue;
	if (findString(attrs, materialSwapKey, materialSwapValue))
		intObj.attributes[materialSwapKey] = materialSwapValue;

	constexpr std::string_view lightmap2Key = "lightMap=";
	std::string_view lightmap2Value;
	if (findString(attrs, lightmap2Key, lightmap2Value))
		intObj.attributes[lightmap2Key] = lightmap2Value;

	constexpr std::string_view lightmap2CoordsKey = "lightMapCoords=";
	std::string_view lightmap2CoordsValue;
	if (findString(attrs, lightmap2CoordsKey, lightmap2CoordsValue))
		intObj.attributes[lightmap2CoordsKey] = lightmap2CoordsValue;
}

size_t
LevelConverter::findAttribute(std::string_view attrs, const std::string_view& key)
{
	size_t offset = 0;
	while ((offset = attrs.find(key, offset)) < attrs.size())
	{
		// Check if the key is an exact match
		if (offset != 0 && (attrs[offset - 1] != '\n' && attrs[offset - 1] != '\0'))
			return std::string::npos;
		return offset;
	}
	return std::string::npos;
}

bool
LevelConverter::findInt(std::string_view attrs, const std::string_view& key, size_t& outValue)
{
	size_t offset = findAttribute(attrs, key);

	if (offset != std::string::npos)
	{
		// Move to end of search string
		offset = offset + key.length();
		size_t end = attrs.find('\n', offset);

		std::string_view valueStr = std::string_view(attrs).substr(offset, end - offset);

		std::from_chars_result result = std::from_chars(valueStr.data(), valueStr.data() + valueStr.length(), outValue);
		if (result.ec != std::errc())
			return false;
	}

	return true;
}

bool
LevelConverter::findFloat(std::string_view attrs, const std::string_view& key, float& outValue)
{
	size_t offset = findAttribute(attrs, key);
	
	if (offset != std::string::npos)
	{
		// Move to end of search string
		offset = offset + key.length();
		size_t end = attrs.find('\n', offset);
		
		std::string_view valueStr = std::string_view(attrs).substr(offset, end - offset);
		std::from_chars_result result = std::from_chars(valueStr.data(), valueStr.data() + valueStr.length(), outValue);
		if (result.ec != std::errc())
			return false;

		return true;
	}

	return false;
}

bool
LevelConverter::findString(std::string_view attrs, const std::string_view& key, std::string_view& outValue)
{
	size_t offset = findAttribute(attrs, key);

	if (offset != std::string::npos)
	{
		// Move to end of search string
		offset = offset + key.length();
		size_t end = attrs.find('\n', offset);
		outValue = std::string_view(attrs).substr(offset, end - offset);
		return true;
	}

	return false;
}

bool
LevelConverter::findStringArray(std::string_view attrs, const std::string_view& key, std::vector<std::string_view>& outValue)
{
	size_t offset = findAttribute(attrs, key);
	
	if (offset != std::string::npos)
	{
		// Move to end of search string
		offset = offset + key.length();
		size_t end = attrs.find('\n', offset);
		// String end is actually }\n
		end = end - 1;
		std::string_view valuePath = std::string_view(attrs).substr(offset, end - offset);

		// Split again, because this is an array...
		size_t subOffset = 0;
		size_t prevSubOffset = 0;
		while ((subOffset = valuePath.find(',', subOffset)) != std::string::npos)
		{
			subOffset += 1;
			outValue.push_back(valuePath.substr(prevSubOffset, subOffset - prevSubOffset - 1));
			prevSubOffset = subOffset;
		}
		// If there is only 1 material path
		// Or append the last in the array (the `while` loop will skip the last one)
		outValue.push_back(valuePath.substr(prevSubOffset));

		return true;
	}

	return false;
}

bool
LevelConverter::findVec3Float(std::string_view attrs, const std::string_view& key, glm::vec3& outValue)
{
	size_t offset = findAttribute(attrs, key);

	if (offset != std::string::npos)
	{
		// Move to end of search string
		offset = offset + key.length();
		size_t end = attrs.find('\n', offset);
		// String end is actually }\n
		end = end - 1;
		std::string_view localPosStr = std::string_view(attrs).substr(offset, end - offset);

		size_t subOffset = 0;
		size_t prevSubOffset = 0;

		// Get X
		subOffset = localPosStr.find(',', subOffset) + 1;
		std::string_view xStr(localPosStr.substr(prevSubOffset, subOffset - prevSubOffset - 1));
		std::from_chars_result result = std::from_chars(xStr.data(), xStr.data() + xStr.length(), outValue.x);
		if (result.ec != std::errc())
			return false;
		prevSubOffset = subOffset;

		// Get Y
		subOffset = localPosStr.find(',', subOffset) + 1;
		std::string_view yStr(localPosStr.substr(prevSubOffset, subOffset - prevSubOffset - 1));
		result = std::from_chars(yStr.data(), yStr.data() + yStr.length(), outValue.y);
		if (result.ec != std::errc())
			return false;
		prevSubOffset = subOffset;

		// Get Z
		subOffset = localPosStr.find(',', subOffset) + 1;
		std::string_view zStr(localPosStr.substr(prevSubOffset, subOffset - prevSubOffset - 1));
		result = std::from_chars(zStr.data(), zStr.data() + zStr.length(), outValue.z);
		if (result.ec != std::errc())
			return false;

		return true;
	}

	return false;
}

void
LevelConverter::fixInternalPath(const LotusLib::LotusPath& internalLevelPath, std::string& outPath)
{
	// Already absolute path
	if (outPath.size() < 5 || outPath[0] == '/')
		return;
	
	outPath = internalLevelPath.parent_path().string() + "/" + internalLevelPath.stem().string() + "/" + outPath;
}