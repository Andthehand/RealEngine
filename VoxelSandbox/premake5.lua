project "VoxelSandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp",
		"vendor/nlohmann/json.hpp"
	}

	includedirs {
		"src",
		"%{wks.location}/RealEngine/vendor/spdlog/include",
		"%{wks.location}/RealEngine/src",
		"%{wks.location}/RealEngine/vendor",
		"%{wks.location}/RealEngine/vendor/imgui",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.ImPlot}",
		"vendor/FastNoise2/include",
		"vendor/nlohmann"
	}

	links {
		"RealEngine"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		defines "RE_DEBUG"
		runtime "Debug"
		symbols "on"
		links {
			"vendor/FastNoise2/lib/FastNoiseD.lib"
		}

		postbuildcommands {
			"{COPYDIR} \"%{LibraryDir.VulkanSDK_DebugDLL}\" \"%{cfg.targetdir}\""
		}

	filter "configurations:Release"
		defines "RE_RELEASE"
		runtime "Release"
		optimize "on"
		links {
			"vendor/FastNoise2/lib/FastNoise.lib"
		}

	filter "configurations:Dist"
		defines "RE_DIST"
		runtime "Release"
		optimize "on"
		links {
			"%{IncludeDir.FastNoise2}/lib/FastNoise.lib"
		}
