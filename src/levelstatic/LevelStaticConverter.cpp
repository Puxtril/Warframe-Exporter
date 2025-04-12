#include "levelstatic/LevelStaticConverter.h"

void
WarframeExporter::LevelStatic::Converter::applyTransformation(LevelStaticObjectExternal& levelObj, std::vector<glm::vec3>& verts)
{
    for (glm::vec3& curVec : verts)
	{
		// Undo mirror by Model extractor
		curVec.z *= -1.0f;

		//curVec.y *= -1.0;

		// Apply level object transforms
		curVec *= glm::vec3(levelObj.scale, levelObj.scale, levelObj.scale);
		curVec = glm::rotate(levelObj.rot, curVec);
		
		// Re-mirror
		curVec.z *= -1.0f;
	}

	//levelObj.scale = 1.0f;
	//levelObj.rot = glm::quat();

	// Reflect model origin point
	// Apply this at export-time
	levelObj.pos.z *= -1.0f;
}