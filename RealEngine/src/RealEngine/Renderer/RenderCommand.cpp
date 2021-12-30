#include "repch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace RealEngine {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}