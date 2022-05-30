// Copyright 2014 DigitalBits Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "util/Logging.h"
#include "util/types.h"

#if defined(USE_SPDLOG)
#include "util/Timer.h"
#include <chrono>
#include <fmt/chrono.h>
#include <fstream>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/spdlog.h>
#endif

namespace digitalbits
{

std::array<std::string const, 14> const Logging::kPartitionNames = {
#define LOG_PARTITION(name) #name,
#include "util/LogPartitions.def"
#undef LOG_PARTITION
};

LogLevel Logging::mGlobalLogLevel = LogLevel::INFO;
std::map<std::string, LogLevel> Logging::mPartitionLogLevels;

#if defined(USE_SPDLOG)
bool Logging::mInitialized = false;
bool Logging::mColor = false;
std::string Logging::mLastPattern;
std::string Logging::mLastFilenamePattern;
#endif

// Right now this is hard-coded to log messages at least as important as INFO
CoutLogger::CoutLogger(LogLevel l) : mShouldLog(l <= Logging::getLogLevel(""))
{
}

CoutLogger::~CoutLogger()
{
    if (mShouldLog)
    {
        std::cout << std::endl;
    }
}

#if defined(USE_SPDLOG)
static spdlog::level::level_enum
convert_loglevel(LogLevel level)
{
    auto slev = spdlog::level::info;
    switch (level)
    {
    case LogLevel::FATAL:
        slev = spdlog::level::critical;
        break;
    case LogLevel::ERR:
        slev = spdlog::level::err;
        break;
    case LogLevel::WARNING:
        slev = spdlog::level::warn;
        break;
    case LogLevel::INFO:
        slev = spdlog::level::info;
        break;
    case LogLevel::DEBUG:
        slev = spdlog::level::debug;
        break;
    case LogLevel::TRACE:
        slev = spdlog::level::trace;
        break;
    default:
        break;
    }
    return slev;
}
#endif

void
Logging::init(bool truncate)
{
#if defined(USE_SPDLOG)
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    if (!mInitialized)
    {
        using namespace spdlog::sinks;
        using std::make_shared;
        using std::shared_ptr;

        auto console = (mColor ? static_cast<shared_ptr<sink>>(
                                     make_shared<stdout_color_sink_mt>())
                               : static_cast<shared_ptr<sink>>(
                                     make_shared<stdout_sink_mt>()));

        std::vector<shared_ptr<sink>> sinks{console};

        if (!mLastFilenamePattern.empty())
        {
            VirtualClock clock(VirtualClock::REAL_TIME);
            std::time_t time = VirtualClock::to_time_t(clock.system_now());
            auto filename =
                fmt::format(mLastFilenamePattern,
                            fmt::arg("datetime", fmt::localtime(time)));

            // NB: We do _not_ pass 'truncate' through to spdlog here -- spdlog
            // interprets 'truncate=true' as a request to open the file in "wb"
            // mode rather than "ab" mode. Opening in "wb" mode does truncate
            // the file but also sets it to a fundamentally different _mode_
            // than "ab" mode.
            //
            // In particular, in "ab" mode the underlying file descriptor is
            // opened with O_APPEND and this makes the kernel atomically preface
            // any write(fd, buf, n) with an lseek(fd, SEEK_END, 0), adjusting
            // the fd's offset to the current end of file, _even if the file
            // shrank_ since the last write.
            //
            // In contrast, in "wb" mode the underlying file descriptor just
            // increments its offset after each write, and if the file shrinks
            // between writes the space between the actual end of file and the
            // offset at the time of writing will be zero-filled!
            //
            // Why would a file shrink? Because users might choose to use an
            // _external_ log-rotation program such as logrotate(1) rather than
            // our internal log rotation command. This command happens to call
            // truncate(2) on the log file after it's made a copy for rotation
            // purposes. This is fine if we're writing to an fd with O_APPEND
            // but it's a recipe for giant zero-filled files if we're not.
            //
            // So instead we just truncate ourselves here if it was requested,
            // and always pass 'truncate=false' to spdlog, so it always opens in
            // "ab" == O_APPEND mode. The "portable" way to truncate is to open
            // an ofstream in out|trunc mode, then immediately close it.
            if (truncate)
            {
                std::ofstream truncator(filename, std::ios_base::out |
                                                      std::ios_base::trunc);
            }

            sinks.emplace_back(
                make_shared<basic_file_sink_mt>(filename, /*truncate=*/false));
        }

        auto makeLogger =
            [&](std::string const& name) -> shared_ptr<spdlog::logger> {
            auto logger =
                make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
            spdlog::register_logger(logger);
            return logger;
        };

        spdlog::set_default_logger(makeLogger("default"));
        for (auto const& partition : digitalbits::Logging::kPartitionNames)
        {
            makeLogger(partition);
        }
        if (mLastPattern.empty())
        {
            mLastPattern = "%Y-%m-%dT%H:%M:%S.%e [%^%n %l%$] %v";
        }
        spdlog::set_pattern(mLastPattern);
        spdlog::set_level(convert_loglevel(mGlobalLogLevel));
        for (auto const& pair : mPartitionLogLevels)
        {
            spdlog::get(pair.first)->set_level(convert_loglevel(pair.second));
        }
        spdlog::flush_every(std::chrono::seconds(1));
        spdlog::flush_on(spdlog::level::err);
        mInitialized = true;
    }
#endif
}

void
Logging::deinit()
{
#if defined(USE_SPDLOG)
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    if (mInitialized)
    {
#define LOG_PARTITION(name) Logging::name##LogPtr = nullptr;
#include "util/LogPartitions.def"
#undef LOG_PARTITION
        spdlog::drop_all();
        mInitialized = false;
    }
#endif
}

void
Logging::setFmt(std::string const& peerID, bool timestamps)
{
#if defined(USE_SPDLOG)
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    init();
    mLastPattern = std::string("%Y-%m-%dT%H:%M:%S.%e ") + peerID +
                   std::string(" [%^%n %l%$] %v");
    spdlog::set_pattern(mLastPattern);
#endif
}

void
Logging::setLoggingToFile(std::string const& filename)
{
#if defined(USE_SPDLOG)
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    mLastFilenamePattern = filename;
    deinit();
    init();
#endif
}

void
Logging::setLoggingColor(bool color)
{
#if defined(USE_SPDLOG)
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    mColor = true;
    deinit();
    init();
#endif
}

void
Logging::setLogLevel(LogLevel level, const char* partition)
{
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    if (partition)
    {
        mPartitionLogLevels[partition] = level;
    }
    else
    {
        mGlobalLogLevel = level;
        mPartitionLogLevels.clear();
    }
#if defined(USE_SPDLOG)
    init();
    auto slev = convert_loglevel(level);
    if (partition)
    {
        spdlog::get(partition)->set_level(slev);
    }
    else
    {
        spdlog::set_level(slev);
    }
#endif
}

LogLevel
Logging::getLLfromString(std::string const& levelName)
{
    if (iequals(levelName, "fatal"))
    {
        return LogLevel::FATAL;
    }

    if (iequals(levelName, "error"))
    {
        return LogLevel::ERR;
    }

    if (iequals(levelName, "warning"))
    {
        return LogLevel::WARNING;
    }

    if (iequals(levelName, "debug"))
    {
        return LogLevel::DEBUG;
    }

    if (iequals(levelName, "trace"))
    {
        return LogLevel::TRACE;
    }

    return LogLevel::INFO;
}

LogLevel
Logging::getLogLevel(std::string const& partition)
{
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    auto p = mPartitionLogLevels.find(partition);
    if (p != mPartitionLogLevels.end())
    {
        return p->second;
    }
    return mGlobalLogLevel;
}

std::string
Logging::getStringFromLL(LogLevel level)
{
    switch (level)
    {
    case LogLevel::FATAL:
        return "Fatal";
    case LogLevel::ERR:
        return "Error";
    case LogLevel::WARNING:
        return "Warning";
    case LogLevel::INFO:
        return "Info";
    case LogLevel::DEBUG:
        return "Debug";
    case LogLevel::TRACE:
        return "Trace";
    }
    return "????";
}

bool
Logging::logDebug(std::string const& partition)
{
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    return mGlobalLogLevel <= LogLevel::DEBUG;
}

bool
Logging::logTrace(std::string const& partition)
{
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    return mGlobalLogLevel <= LogLevel::TRACE;
}

void
Logging::rotate()
{
    std::lock_guard<std::recursive_mutex> guard(mLogMutex);
    deinit();
    init(/*truncate=*/true);
}

// throws if partition name is not recognized
std::string
Logging::normalizePartition(std::string const& partition)
{
    return partition;
}

std::recursive_mutex Logging::mLogMutex;

#if defined(USE_SPDLOG)
#define LOG_PARTITION(name) \
    LogPtr Logging::name##LogPtr = nullptr; \
    LogPtr Logging::get##name##LogPtr() \
    { \
        std::lock_guard<std::recursive_mutex> guard(mLogMutex); \
        if (!name##LogPtr) \
        { \
            name##LogPtr = spdlog::get(#name); \
        } \
        return name##LogPtr; \
    }
#include "util/LogPartitions.def"
#undef LOG_PARTITION
#endif
}
