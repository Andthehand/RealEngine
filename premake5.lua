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
IncludeDir["ImGui"] = "RealEngine/vendor/imgui"
IncludeDir["glm"] = "RealEngine/vendor/glm"

group "Dependencies"

	include "RealEngine/vendor/GLFW"
	include "RealEngine/vendor/Glad"
	include "RealEngine/vendor/imgui"

group ""

project "RealEngine"
	location "RealEngine"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "repch.h"
	pchsource "RealEngine/src/repch.cpp"

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}"
	}

	links {
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		
		defines {
			"RE_PLATFORM_WINDOWS",
			"RE_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands {
			("{copy} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}


		filter "configurations:Debug"
			defines "RE_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "RE_RELEASE"
			runtime "Release"
			optimize"On"

		filter "configurations:DIST"
			defines "RE_DIST"
			runtime "Release"
			optimize"On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleAPP"
	language "C++"
	staticruntime "off"

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
		cppdialect "C++17"
		systemversion "latest"
		
		defines {
			"RE_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "RE_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "RE_RELEASE"
			runtime "Release"
			optimize"On"

		filter "configurations:DIST"
			defines "RE_DIST"
			runtime "Release"
			optimize"On"