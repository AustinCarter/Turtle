workspace "Turtle"
	architecture "x64"
	startproject "Tortuga"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["yaml_cpp"] = "Turtle/vendor/yaml-cpp/include"
IncludeDir["GLFW"] = "Turtle/vendor/GLFW/include"
IncludeDir["Glad"] = "Turtle/vendor/Glad/include"
IncludeDir["ImGui"] = "Turtle/vendor/imgui"
IncludeDir["glm"] = "Turtle/vendor/glm"
IncludeDir["stb_image"] = "Turtle/vendor/stb_image"
IncludeDir["entt"] = "Turtle/vendor/entt/include"
IncludeDir["lua"] = "Turtle/vendor/lua/src"
IncludeDir["miniaudio"] = "Turtle/vendor/miniaudio/include"

group "Dependencies"
	include "Turtle/vendor/GLFW"
	include "Turtle/vendor/Glad"
	include "Turtle/vendor/imgui"
	include "Turtle/vendor/yaml-cpp"
	include "Turtle/vendor/lua"
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
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.lua}",
		"%{IncludeDir.miniaudio}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"lua",
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.miniaudio}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.lua}"
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
		"%{IncludeDir.entt}",
		"%{IncludeDir.miniaudio}",
		"%{IncludeDir.yaml_cpp}",
		"%{IncludeDir.lua}"
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

