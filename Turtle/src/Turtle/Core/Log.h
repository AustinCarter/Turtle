#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Turtle {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define TURT_CORE_TRACE(...)    ::Turtle::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define TURT_CORE_INFO(...)     ::Turtle::Log::GetCoreLogger()->info(__VA_ARGS__)
#define TURT_CORE_WARN(...)     ::Turtle::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define TURT_CORE_ERROR(...)    ::Turtle::Log::GetCoreLogger()->error(__VA_ARGS__)
#define TURT_CORE_FATAL(...)    ::Turtle::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define TURT_TRACE(...)	      ::Turtle::Log::GetClientLogger()->trace(__VA_ARGS__)
#define TURT_INFO(...)	      ::Turtle::Log::GetClientLogger()->info(__VA_ARGS__)
#define TURT_WARN(...)	      ::Turtle::Log::GetClientLogger()->warn(__VA_ARGS__)
#define TURT_ERROR(...)	      ::Turtle::Log::GetClientLogger()->error(__VA_ARGS__)
#define TURT_FATAL(...)	      ::Turtle::Log::GetClientLogger()->fatal(__VA_ARGS__)