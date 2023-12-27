#pragma once

#include "RealEngine/Core/Application.h"
#include "RealEngine/Core/Layer.h"
#include "RealEngine/Core/Log.h"
#include "RealEngine/Core/Assert.h"

#include "RealEngine/Core/TimeStep.h"

#include "RealEngine/Core/CircularBuffer.h"

#include "RealEngine/Core/Input.h"
#include "RealEngine/Core/KeyCodes.h"
#include "RealEngine/Core/MouseCodes.h"

#include "RealEngine/ImGui/ImGuiLayer.h"
#include "RealEngine/ImGui/ImGuiCustom.h"

#include "RealEngine/Scene/Scene.h"
#include "RealEngine/Scene/Entity.h"
#include "RealEngine/Scene/ScriptableEntity.h"
#include "RealEngine/Scene/Components.h"

#include "RealEngine/Project/Project.h"

#include "RealEngine/Utils/YAML.h"

#include "RealEngine/Utils/Threads/JobQueue.h"
#include "RealEngine/Utils/Variant.h"
#include "RealEngine/Utils/StringBuilder.h"

// ---Renderer------------------------
#include "RealEngine/Renderer/Renderer.h"
#include "RealEngine/Renderer/Renderer2D.h"
#include "RealEngine/Renderer/RenderCommand.h"

#include "RealEngine/Renderer/Buffer.h"
#include "RealEngine/Renderer/VertexArray.h"
#include "RealEngine/Renderer/Shader.h"
#include "RealEngine/Renderer/Framebuffer.h"
#include "RealEngine/Renderer/Texture.h"

#include "RealEngine/Renderer/OrthographicCamera.h"
#include "RealEngine/Renderer/OrthographicCameraController.h"
// -----------------------------------
