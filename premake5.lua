workspace "RealEngine"
	architecture "x64"
	startproject "Sandbox"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "RealEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "RealEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "RealEngine/vendor/imgui"
IncludeDir["glm"] = "RealEngine/vendor/glm"
IncludeDir["stb_image"] = "RealEngine/vendor/stb_image"

group "Dependencies"

	include "RealEngine/vendor/GLFW"
	include "RealEngine/vendor/Glad"
	include "RealEngine/vendor/imgui"

group ""

project "RealEngine"
	location "RealEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "repch.h"
	pchsource "RealEngine/src/repch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	defines {
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines {
			"RE_PLATFORM_WINDOWS",
			"RE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}


		filter "configurations:Debug"
			defines "RE_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "RE_RELEASE"
			runtime "Release"
			optimize"on"

		filter "configurations:DIST"
			defines "RE_DIST"
			runtime "Release"
			optimize"on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleAPP"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"RealEngine/vendor/spdlog/include",
		"RealEngine/src",
		"RealEngine/vendor",
		"%{IncludeDir.glm}"
	}

	links {
		"RealEngine"
	}

	filter "system:windows"
		systemversion "latest"
		
		defines {
			"RE_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "RE_DEBUG"
			runtime "Debug"
			symbols "on"

		filter "configurations:Release"
			defines "RE_RELEASE"
			runtime "Release"
			optimize"on"

		filter "configurations:DIST"
			defines "RE_DIST"
			runtime "Release"
			optimize"on"