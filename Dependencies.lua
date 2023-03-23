-- RealEngine Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}
IncludeDir["stb_image"] = "%{wks.location}/RealEngine/vendor/stb_image"
IncludeDir["yaml_cpp"] = "%{wks.location}/RealEngine/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/RealEngine/vendor/Box2D/include"
IncludeDir["GLFW"] = "%{wks.location}/RealEngine/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/RealEngine/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/RealEngine/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/RealEngine/vendor/ImGuizmo"
IncludeDir["ImPlot"] = "%{wks.location}/RealEngine/vendor/ImPlot"
IncludeDir["glm"] = "%{wks.location}/RealEngine/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/RealEngine/vendor/entt/include"
IncludeDir["shaderc"] = "%{wks.location}/RealEngine/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/RealEngine/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}
Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
