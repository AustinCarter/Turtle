#pragma once

#include <memory>

#ifdef TURT_DEBUG
	#define TURT_ENABLE_ASSERTS
#endif

#ifdef TURT_ENABLE_ASSERTS
	#define TURT_ASSERT(x, ...) { if(!(x)) { TURT_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TURT_CORE_ASSERT(x, ...) { if(!(x)) { TURT_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define TURT_STATIC_ASSERT(expr) static_assert(expr, "Static assert failed: " #expr);
#else
	#define TURT_ASSERT(x, ...)
	#define TURT_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define TURT_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace Turtle {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}