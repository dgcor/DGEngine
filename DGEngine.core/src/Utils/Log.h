#pragma once

#if !defined(DGENGINE_HAS_LOGGING) || !__has_include(<spdlog/spdlog.h>)
#    undef DGENGINE_HAS_LOGGING
#    define DGENGINE_HAS_LOGGING 0
#endif

#define DGENGINE_INIT_LOGGING() (void)0

#if DGENGINE_HAS_LOGGING
#    if _DEBUG
#        define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_TRACE
#    elif DGENGINE_ENABLE_LOGGING
#        define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_INFO
#    else
#        undef DGENGINE_HAS_LOGGING
#        define DGENGINE_HAS_LOGGING 0
#        define SPDLOG_ACTIVE_LEVEL SPDLOG_LEVEL_OFF
#    endif
#endif

#if DGENGINE_HAS_LOGGING
#    include <spdlog/spdlog.h>
#    if SPDLOG_ACTIVE_LEVEL < SPDLOG_LEVEL_INFO
#        undef DGENGINE_INIT_LOGGING
#        define DGENGINE_INIT_LOGGING() spdlog::set_level((spdlog::level::level_enum)SPDLOG_ACTIVE_LEVEL)
#    endif
#else
#    define SPDLOG_LOGGER_TRACE(logger, ...) (void)0
#    define SPDLOG_TRACE(...) (void)0

#    define SPDLOG_LOGGER_DEBUG(logger, ...) (void)0
#    define SPDLOG_DEBUG(...) (void)0

#    define SPDLOG_LOGGER_INFO(logger, ...) (void)0
#    define SPDLOG_INFO(...) (void)0

#    define SPDLOG_LOGGER_WARN(logger, ...) (void)0
#    define SPDLOG_WARN(...) (void)0

#    define SPDLOG_LOGGER_ERROR(logger, ...) (void)0
#    define SPDLOG_ERROR(...) (void)0

#    define SPDLOG_LOGGER_CRITICAL(logger, ...) (void)0
#    define SPDLOG_CRITICAL(...) (void)0
#endif
