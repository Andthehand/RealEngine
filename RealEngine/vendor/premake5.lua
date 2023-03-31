project "optick"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
    	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	flags { "NoManifest", "FatalWarnings" }

	files {
		"src/**.h",
		"src/**.cpp"
	}
	
	includedirs {
		"src"
	}

	vpaths {
		["api"] = { 
			"src/optick.h",
			"src/optick.config.h",
		},
	}

	defines { 
		USE_OPTICK=1,
		OPTICK_FIBERS=1,
		MT_INSTRUMENTED_BUILD,
		_CRT_SECURE_NO_WARNINGS,
		OPTICK_LIB=1,
		OPTICK_EXPORTS,
		OPTICK_ENABLE_GPU_D3D12=0,
		PTICK_ENABLE_GPU_VULKAN=0
	}

USE_OPTICK=1
OPTICK_FIBERS=1
NDEBUG
MT_INSTRUMENTED_BUILD
_CRT_SECURE_NO_WARNINGS
OPTICK_LIB=1
OPTICK_EXPORTS
OPTICK_ENABLE_GPU_D3D12=0
OPTICK_ENABLE_GPU_VULKAN=0

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
		defines {
			_DEBUG,
			_CRTDBG_MAP_ALLOC
		}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
		defines {
			NDEBUG
		}		
		flags { optimization_flags }

    filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		defines {
			NDEBUG
		}	
        	symbols "off"
		flags { optimization_flags }
