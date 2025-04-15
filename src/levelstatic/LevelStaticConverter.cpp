#include "levelstatic/LevelStaticConverter.h"

void
WarframeExporter::LevelStatic::Converter::applyTransformation(LevelStaticObjectExternal& levelObj)
{
	// Reflect model origin point
	// Apply this at export-time
	levelObj.pos.z *= -1.0f;
}

void
WarframeExporter::LevelStatic::Converter::applyTransformation(std::vector<glm::vec3>& verts)
{
    for (glm::vec3& curVec : verts)
	{
		// Undo mirror by Model extractor
		curVec.z *= -1.0f;
	}
}