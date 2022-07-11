#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

std::shared_ptr<spdlog::logger> Log::logger;

void Log::Init() {
	auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
	sink->set_pattern("%^[%T] [%l] %v%$");
	logger = std::make_shared<spdlog::logger>("core", sink);
	spdlog::register_logger(logger);
	logger->set_level(spdlog::level::info);
}
