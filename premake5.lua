include "./vendor/premake/premake_customization/solution_items.lua"

workspace "RealEngine"
	architecture "x86_64"
	startproject "RealEngine-Editor"

	configurations {
		"Debug",
		"Release",
		"Dist"
	}

	solution_items {
		".editorconfig"
	}

	flags {
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/RealEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/RealEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/RealEngine/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/RealEngine/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/RealEngine/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/RealEngine/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/RealEngine/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/RealEngine/vendor/ImGuizmo"
IncludeDir["ImPlot"] = "%{wks.location}/RealEngine/vendor/ImPlot"

group "Dependencies"
	include "vendor/premake"
	include "RealEngine/vendor/GLFW"
	include "RealEngine/vendor/Glad"
	include "RealEngine/vendor/imgui"
	include "RealEngine/vendor/yaml-cpp"
group ""

include "RealEngine"
include "Sandbox"
include "RealEngine-Editor"
include "MachineLearningSandbox"
include "VoxelSandbox"
