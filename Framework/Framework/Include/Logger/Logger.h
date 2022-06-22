#pragma once

#include <spdlog/spdlog.h>

#define LOGGER_NAME "Logger"

namespace FW::Logger
{
	void Initialize();
	void Release();
}

#if defined(PLATFORM_WINDOWS)
	#define DEBUG_BREAK { __debugbreak(); abort(); }
#elif defined(PLATFORM_MAC)
	#define DEBUG_BREAK __builtin_debugtrap();
#endif

#define LOG_TRACE(...) 		if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->trace(__VA_ARGS__); }
#define LOG_DEBUG(...) 		if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->debug(__VA_ARGS__); }
#define LOG_INFO(...) 		if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->info(__VA_ARGS__); }
#define LOG_WARN(...) 		if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->warn(__VA_ARGS__); }
#define LOG_ERROR(...) 		if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->error(__VA_ARGS__); }
#define LOG_FATAL(...) 		if (spdlog::get(LOGGER_NAME) != nullptr) { spdlog::get(LOGGER_NAME)->critical(__VA_ARGS__); }
#define LOG_ASSERT(x, msg) 	if (x) {} else { LOG_FATAL("ASSERT - {}\n\t{}\n\tin file: {}\n\ton line: {}", #x, msg, __FILE__, __LINE__); DEBUG_BREAK }