#include "repch.h"
#include "UniformBuffer.h"

#include "RealEngine/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLUniformBuffer.h"

namespace RealEngine {

	Ref<UniformBuffer> UniformBuffer::Create(uint32_t size, uint32_t binding) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLUniformBuffer>(size, binding);
		}

		RE_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

}
