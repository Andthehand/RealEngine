workspace "RealEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "RealEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "RealEngine/vendor/Glad/include"

include "RealEngine/vendor/GLFW"
include "RealEngine/vendor/Glad"

project "RealEngine"
	location "RealEngine"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "repch.h"
	pchsource "RealEngine/src/repch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}"
	}

	links {
		"GLFW",
		"Glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines {
			"RE_PLATFORM_WINDOWS",
			"RE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{copy} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

		filter "configurations:Debug"
			defines "RE_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "RE_RELEASE"
			buildoptions "/MD"
			optimize"On"

		filter "configurations:DIST"
			defines "RE_DIST"
			buildoptions "/MD"
			optimize"On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleAPP"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"RealEngine/vendor/spdlog/include",
		"RealEngine/src"
	}

	links {
		"RealEngine"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"
		
		defines {
			"RE_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "RE_DEBUG"
			buildoptions "/MDd"
			symbols "On"

		filter "configurations:Release"
			defines "RE_RELEASE"
			buildoptions "/MD"
			optimize"On"

		filter "configurations:DIST"
			defines "RE_DIST"
			buildoptions "/MD"
			optimize"On"