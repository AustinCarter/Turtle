#include "turtpch.h"
#include "Window.h"

#ifdef TURT_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif
namespace Turtle
{

	Scope<Window> Window::Create(const WindowProps& props)
	{
	#ifdef TURT_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
	#else
		TURT_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
}