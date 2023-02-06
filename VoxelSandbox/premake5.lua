project "VoxelSandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"src/**.h",
		"src/**.cpp"
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
		"%{IncludeDir.FastNoise2}/include"
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
			"%{IncludeDir.FastNoise2}/lib/FastNoiseD.lib"
		}

	filter "configurations:Release"
		defines "RE_RELEASE"
		runtime "Release"
		optimize "on"
		links {
			"%{IncludeDir.FastNoise2}/lib/FastNoise.lib"
		}

	filter "configurations:Dist"
		defines "RE_DIST"
		runtime "Release"
		optimize "on"
		links {
			"%{IncludeDir.FastNoise2}/lib/FastNoise.lib"
		}
