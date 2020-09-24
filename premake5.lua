workspace "Turtle"
	architecture "x64"
	startproject "Tortuga"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Turtle/vendor/GLFW/include"
IncludeDir["Glad"] = "Turtle/vendor/Glad/include"
IncludeDir["ImGui"] = "Turtle/vendor/imgui"
IncludeDir["glm"] = "Turtle/vendor/glm"
IncludeDir["stb_image"] = "Turtle/vendor/stb_image"
IncludeDir["entt"] = "Turtle/vendor/entt/include"

group "Dependencies"
	include "Turtle/vendor/GLFW"
	include "Turtle/vendor/Glad"
	include "Turtle/vendor/imgui"
group ""

project "Turtle"
	location "Turtle"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "turtpch.h"
	pchsource "Turtle/src/turtpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/glm/glm/**.hpp"
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs 
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TURT_PLATFORM_WINDOWS",
			"TURT_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "TURT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TURT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TURT_DIST"
		runtime "Release"
		optimize "on"

project "Tortuga"
	location "Tortuga"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Turtle/vendor/spdlog/include",
		"Turtle/src",
		"Turtle/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Turtle"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TURT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TURT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TURT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TURT_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Turtle/vendor/spdlog/include",
		"Turtle/src",
		"Turtle/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Turtle"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"TURT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "TURT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "TURT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "TURT_DIST"
		runtime "Release"
		optimize "on"

