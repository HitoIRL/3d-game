#pragma once

#include <spdlog/spdlog.h>

namespace Log {
	void Init();

	extern std::shared_ptr<spdlog::logger> logger;
}

// inspired from hazel engine <3
#define LOG_INFO(...) Log::logger->info(__VA_ARGS__)
#define LOG_WARN(...) Log::logger->warn(__VA_ARGS__)
#define LOG_ERROR(...) Log::logger->error(__VA_ARGS__)
