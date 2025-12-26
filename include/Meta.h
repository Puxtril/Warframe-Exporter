#pragma once

#include <string_view>
#include "spdlog/spdlog.h"

const static std::string_view g_version = "dev";
const static spdlog::level::level_enum g_logLevel = spdlog::level::debug;
