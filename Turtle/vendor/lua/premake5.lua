project "lua"
	kind "StaticLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/*.c",
		"src/*.h",
		"src/*.hpp",
		"src/*.cpp"

		-- "src/lapi.c",
		-- "src/lapi.h",
		-- "src/lauxlib.c",
		-- "src/lauxlib.h",
		-- "src/lbaselib.c",
		-- "src/lbitlib.c",
		-- "src/lcode.c",
		-- "src/lcode.h",
		-- "src/lcorolib.c",
		-- "src/lctype.c",
		-- "src/lctype.h",
		-- "src/ldblib.c",
		-- "src/ldebug.c",
		-- "src/ldebug.h",
		-- "src/ldo.c",
		-- "src/ldo.h",
		-- "src/ldump.c",
		-- "src/lfunc.c",
		-- "src/lfunc.h",
		-- "src/lgc.c",
		-- "src/lgc.h",
		-- "src/linit.c",
		-- "src/liolib.c",
		-- "src/llex.c",
		-- "src/llex.h",
		-- "src/llimits.h",
		-- "src/lmathlib.c",
		-- "src/lmem.c",
		-- "src/lmem.h",
		-- "src/loadlib.c",
		-- "src/lobject.c",
		-- "src/lobject.h",
		-- "src/lopcodes.c",
		-- "src/lopcodes.h",
		-- "src/loslib.c",
		-- "src/lparser.c",
		-- "src/lparser.h",
		-- "src/lprefix.h",
		-- "src/lstate.c",
		-- "src/lstate.h",
		-- "src/lstring.c",
		-- "src/lstring.h",
		-- "src/lstrlib.c",
		-- "src/ltable.c",
		-- "src/ltable.h",
		-- "src/ltablib.c",
		-- "src/ltm.c",
		-- "src/ltm.h",
		-- --"src/lua.c",
		-- --"src/lua.h",
		-- "src/lua.hpp",
		-- --"src/luac.c",
		-- "src/luaconf.h",
		-- "src/lualib.h",
		-- "src/lundump.c",
		-- "src/lundump.h",
		-- "src/lutf8lib.c",
		-- "src/lvm.c",
		-- "src/lvm.h",
		-- "src/lzio.c",
		-- "src/lzio.h"
	}

	filter "system:windows"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "system:linux"
		pic "On"
		systemversion "latest"
		cppdialect "C++17"
		staticruntime "On"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
