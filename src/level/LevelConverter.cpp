#include "level/LevelConverter.h"

using namespace WarframeExporter::Level;

void
LevelConverter::convertToInternal(LevelHeaderExternal& extHeader, LevelBodyExternal& extBody, const LotusLib::LotusPath& internalLevelPath, LevelInternal& intBody)
{
	intBody._rawAttributeString = std::move(extBody.attributes);
	std::vector<LevelConverter::SplitAttributes> splitAttrs = splitAttributes(intBody._rawAttributeString);

	// Why does DE index like this...
	std::map<int, LevelConverter::SplitAttributes> instanceMap;
	for (int x = 0; x < splitAttrs.size(); x++)
	{
		instanceMap[splitAttrs[x].instance] = splitAttrs[x];
	}
	
	intBody.objs.resize(extHeader.levelObjs.size());
	for (size_t x = 0; x < intBody.objs.size(); x++)
	{
		LevelObjectHeaderExternal& extObj = extHeader.levelObjs[x];
		LevelObjectInternal& intObj = intBody.objs[x];
		
		intObj.objTypePath = std::move(extObj.objTypePath);
		intObj.objName = std::move(extObj.objName);
		intObj.pos = std::move(extObj.pos);
		intObj.rot = std::move(extObj.rot);

		// It all comes together here
		LevelConverter::SplitAttributes& curAttrs = instanceMap[extObj.meshInstanceIndex];
		fixInternalPath(internalLevelPath, curAttrs.meshPath, intObj.meshPath);
		intObj.materials = std::move(curAttrs.materials);
		intObj.scale = std::move(curAttrs.scale);
		intObj.attributes = std::move(curAttrs.extraAttrs);
		//intObj.pos += curAttrs.posOffset;
	}
}

std::vector<LevelConverter::SplitAttributes>
LevelConverter::splitAttributes(const std::string& attrs)
{
	size_t prevPos = 0;
	size_t pos = 0;
	std::vector<LevelConverter::SplitAttributes> splitAttrs;
	
	while ((pos = attrs.find('\0', pos+1)) != std::string::npos) {
		std::string_view curAttrs = std::string_view(attrs).substr(prevPos, pos - prevPos);
		
		LevelConverter::SplitAttributes curAttr;

		constexpr std::string_view meshKey = "Mesh=";
		if (!findString(curAttrs, meshKey, curAttr.meshPath))
			curAttr.meshPath = "";

		constexpr std::string_view materialOverrideKey = "OverrideMaterial={";
		if (!findStringArray(curAttrs, materialOverrideKey, curAttr.materials))
			curAttr.materials = {};

		/*
		* I don't know if this is needed...
		constexpr std::string_view posOffsetKey = "";
		if (!findVec3Float(curAttrs, posOffsetKey, curAttr.posOffset))
		{
			curAttr.posOffset = { 0.0, 0.0, 0.0 };
		}
		*/

		constexpr std::string_view scaleKey = "MeshScale=";
		if (!findFloat(curAttrs, scaleKey, curAttr.scale))
		{
			curAttr.scale = 1.0;
		}

		// Very important
		// If this doesn't have an index, it's not a mesh
		// There are scripts, triggers, sounds, etc as well
		constexpr std::string_view instanceKey = "Instance=";
		if (!findInt(curAttrs, instanceKey, curAttr.instance))
		{
			if (curAttr.meshPath.length() > 0)
				WarframeExporter::Logger::getInstance().warn("Mesh path attribute is present, but no Instance attribute was found");
			curAttr.instance = -1;
		}

		constexpr std::string_view lightmap1Key = "LightMap=";
		std::string_view lightMapvalue;
		if (findString(curAttrs, lightmap1Key, lightMapvalue))
			curAttr.extraAttrs[lightmap1Key] = lightMapvalue;

		constexpr std::string_view lightmapCoordsKey = "LightMapCoords=";
		std::string_view lightMapCordsValue;
		if (findString(curAttrs, lightmapCoordsKey, lightMapCordsValue))
			curAttr.extraAttrs[lightmapCoordsKey] = lightMapCordsValue;

		constexpr std::string_view lightmapNormalizeKey = "LightMapNormalize=";
		std::string_view lightMapNormalizeValue;
		if (findString(curAttrs, lightmapNormalizeKey, lightMapNormalizeValue))
			curAttr.extraAttrs[lightmapNormalizeKey] = lightMapNormalizeValue;

		constexpr std::string_view materialSwapKey = "MaterialForSwap=";
		std::string_view materialSwapValue;
		if (findString(curAttrs, materialSwapKey, materialSwapValue))
			curAttr.extraAttrs[materialSwapKey] = materialSwapValue;

		constexpr std::string_view lightmap2Key = "lightMap=";
		std::string_view lightmap2Value;
		if (findString(curAttrs, lightmap2Key, lightmap2Value))
			curAttr.extraAttrs[lightmap2Key] = lightmap2Value;

		constexpr std::string_view lightmap2CoordsKey = "lightMapCoords=";
		std::string_view lightmap2CoordsValue;
		if (findString(curAttrs, lightmap2CoordsKey, lightmap2CoordsValue))
			curAttr.extraAttrs[lightmap2CoordsKey] = lightmap2CoordsValue;

		splitAttrs.push_back(curAttr);

		prevPos = pos;
	}
	
	return splitAttrs;
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
LevelConverter::fixInternalPath(const LotusLib::LotusPath& internalLevelPath, std::string_view readPath, std::string& outPath)
{
	// Already absolute path
	if (readPath.size() < 5 || readPath[0] == '/')
	{
		outPath = std::string(readPath);
		return;
	}
	
	outPath = internalLevelPath.parent_path().string() + "/" + internalLevelPath.stem().string() + "/" + std::string(readPath.data(), readPath.length());
}