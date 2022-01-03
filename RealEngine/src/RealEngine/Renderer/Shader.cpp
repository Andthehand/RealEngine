#include "repch.h"
#include "Shader.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace RealEngine {
	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragmentSrc) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return new OpenGLShader(vertexSrc, fragmentSrc);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}