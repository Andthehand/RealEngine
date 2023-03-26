#include "repch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace RealEngine {
	Scope<RendererAPI> RenderCommand::s_RendererAPI = nullptr;
}
