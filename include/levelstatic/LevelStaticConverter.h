#pragma once

#include <string_view>
#include <vector>
#include "LotusNotationParser.h"

#include "LotusPath.h"
#include "level/LevelStructs.h"
#include "levelstatic/LevelStaticStructs.h"
#include "model/ModelStructs.hpp"
#include "ExporterLogger.h"
#include "levelstatic/LevelStaticStructs.h"

namespace WarframeExporter::LevelStatic::Converter
{
    void applyTransformation(LevelStaticObjectExternal& levelObj);
    void applyTransformation(std::vector<glm::vec3>& verts);
};