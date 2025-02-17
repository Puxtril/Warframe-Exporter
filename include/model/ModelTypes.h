#pragma once

namespace WarframeExporter::Model
{
    enum class ModelType
    {
        MODEL_STATIC_86 = 86,
        MODEL_STATIC_96 = 96,
        MODEL_LEVEL_99 = 99,
        MODEL_DCM_101 = 101,
        MODEL_HLOD_OR_DCM_102 = 102,
        MODEL_LEVEL_103 = 103,
        MODEL_HLOD_OR_DCM_105 = 105,
        MODEL_LEVEL_106 = 106,
        MODEL_TERRAIN_159 = 159,
        MODEL_TERRAIN_160 = 160,
        MODEL_TERRAIN_163 = 163,
        MODEL_RIGGED_269 = 269,
        MODEL_RIGGED_272 = 272,
        MODEL_RIGGED_275 = 275,
        MODEL_RIGGED_277 = 277,
        MODEL_RIGGED_278 = 278,
        MODEL_PACKED_289 = 289,
        MODEL_PACKED_290 = 290,
        MODEL_PACKED_300 = 300,
        MODEL_PACKED_301 = 301
    };

    enum class ScaleType
    {
        XYZ,
        XZ,
        NONE
    };
}