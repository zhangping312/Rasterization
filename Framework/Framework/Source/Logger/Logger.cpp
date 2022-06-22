#include <Logger/Logger.h>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace FW::Logger
{
	void Initialize()
	{
		// Set pattern:
		// 		%^**%$ = color begin and color end
		// 		%Y-%m-%d %H:%M:%S.%e = time
		// 		%n = logger name
		// 		%v = message
		// 		%l = debug level
		auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		consoleSink->set_pattern("%^[%Y-%m-%d %H:%M:%S.%e] %n: %v%$");
		auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.log", true);
		fileSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %n: %v");

		std::vector<spdlog::sink_ptr> sinks { consoleSink, fileSink };

		// Register core logger
		auto logger = std::make_shared<spdlog::logger>(LOGGER_NAME, sinks.begin(), sinks.end());
		logger->set_level(spdlog::level::trace);
		logger->flush_on(spdlog::level::trace);
		spdlog::register_logger(logger);
	}

	void Release()
	{
		spdlog::shutdown();
	}
}