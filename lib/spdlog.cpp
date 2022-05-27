// This file exists to compile spdlog's out-of-line compiled parts in the
// context of the customizations made in our own SpdlogTweaks.h file.

#include "src/util/SpdlogTweaks.h"

#include "lib/spdlog/src/async.cpp"
#include "lib/spdlog/src/cfg.cpp"
#include "lib/spdlog/src/color_sinks.cpp"
#include "lib/spdlog/src/file_sinks.cpp"
#include "lib/spdlog/src/fmt.cpp"
#include "lib/spdlog/src/spdlog.cpp"
#include "lib/spdlog/src/stdout_sinks.cpp"
