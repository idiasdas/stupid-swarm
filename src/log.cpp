#include "log.h"

std::shared_ptr<spdlog::logger> Log::_logger;

void Log::Init()
{
    spdlog::set_pattern("[%l]:%$ %v");
    _logger = spdlog::stdout_color_mt("Logger");
    _logger->set_level(spdlog::level::trace);
}
