#include "repch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace RealEngine {
	VertexArray* VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::OpenGL:  return new OpenGLVertexArray();
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

}