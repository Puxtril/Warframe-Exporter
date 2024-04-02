#pragma once

namespace WarframeExporter::Model
{
    enum class ModelType
    {
        MODEL_STATIC_86 = 86,
        MODEL_STATIC_96 = 96,
        MODEL_LEVEL_99 = 99,
        MODEL_DCM_101 = 101,
        MODEL_HLOD_OR_DCM_102 = 102, // Thanks, DE!
        MODEL_LEVEL_103 = 103,
        MODEL_TERRAIN_159 = 159,
        MODEL_TERRAIN_160 = 160,
        MODEL_RIGGED_269 = 269,
        MODEL_RIGGED_272 = 272,
        MODEL_PACKED_289 = 289,
        MODEL_PACKED_290 = 290
    };

    enum class ScaleType
    {
        XYZ,
        XZ,
        NONE
    };
}