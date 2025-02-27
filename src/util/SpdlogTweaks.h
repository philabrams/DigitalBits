#pragma once

// Copyright 2020 DigitalBits Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#define SPDLOG_COMPILED_LIB
#define SPDLOG_FMT_EXTERNAL
#define SPDLOG_NO_THREAD_ID
#define SPDLOG_NO_TLS
#define SPDLOG_NO_ATOMIC_LEVELS
#define SPDLOG_PREVENT_CHILD_FD
#define SPDLOG_LEVEL_NAMES \
    { \
        "TRACE", "DEBUG", "INFO", "WARNING", "ERROR", "FATAL", "OFF" \
    }
#define SPDLOG_SHORT_LEVEL_NAMES \
    { \
        "T", "D", "I", "W", "E", "F", "O" \
    }
#define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
