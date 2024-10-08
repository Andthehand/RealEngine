#include "repch.h"
#include "VertexArray.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace RealEngine {
	Ref<VertexArray> VertexArray::Create() {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}
