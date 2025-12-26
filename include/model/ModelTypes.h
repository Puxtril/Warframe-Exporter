#pragma once

namespace WarframeExporter::Model
{
    enum class ModelType
    {
        MODEL_STATIC_86 = 86,
        MODEL_STATIC_96 = 96,
        MODEL_STATIC_102 = 102,
        MODEL_LEVEL1_99 = 99,
        MODEL_LEVEL2_103 = 103,
        MODEL_LEVEL1_105 = 105, // Also static model in Soulframe
        MODEL_LEVEL_106 = 106,
        MODEL_LEVEL2_109 = 109,
        MODEL_LEVEL2_112 = 112,
        MODEL_TERRAIN_159 = 159,
        MODEL_TERRAIN_160 = 160,
        MODEL_TERRAIN_163 = 163,
        MODEL_TERRAIN_166 = 166,
        MODEL_TERRAIN_169 = 169,
        MODEL_RIGGED_269 = 269,
        MODEL_RIGGED_272 = 272,
        MODEL_RIGGED_275 = 275,
        MODEL_RIGGED_277 = 277,
        MODEL_RIGGED_278 = 278,
        MODEL_RIGGED_281 = 281,
        MODEL_RIGGED_283 = 283,
        MODEL_RIGGED_287 = 287,
        MODEL_PACKED_289 = 289,
        MODEL_PACKED_290 = 290,
        MODEL_PACKED_299 = 299,
        MODEL_PACKED_300 = 300,
        MODEL_PACKED_301 = 301,
        MODEL_PACKED_310 = 310,
        MODEL_PACKED_314 = 314
    };

    // Appeared in Soulframe late 2025
    // Successor of Level1
    enum class ModelSMType
    {
        MODEL_SM_105 = 105,
        MODEL_SM_108 = 108,
    };

    enum class ModelDCMType
    {
        MODEL_DCM_101 = 101,
        MODEL_DCM_102 = 102,
        MODEL_DCM_105 = 105,
        MODEL_DCM_108 = 108,
        MODEL_DCM_111 = 111,
    };

    enum class ModelHLODType
    {
        MODEL_HLOD_102 = 102,
        MODEL_HLOD_105 = 105,
        MODEL_HLOD_108 = 108,
        MODEL_HLOD_111 = 111,
    };

    enum class ScaleType
    {
        XYZ,
        XZ,
        NONE
    };
}