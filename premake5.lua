 include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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

group "Dependencies"
	include "vendor/premake"
	include "RealEngine/vendor/GLFW"
	include "RealEngine/vendor/Glad"
	include "RealEngine/vendor/imgui"
	include "RealEngine/vendor/yaml-cpp"
	include "RealEngine/vendor/Box2D"
group ""

group "Core"
	include "RealEngine"
	include "RealEngine-ScriptCore"
group ""

group "Tools"
	include "RealEngine-Editor"
group ""

group "Misc"
	include "Sandbox"
	include "MachineLearningSandbox"
	include "VoxelSandbox"
group ""