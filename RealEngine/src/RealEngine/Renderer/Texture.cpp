#include "repch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace RealEngine {
	Ref<Texture2D> Texture2D::Create(const uint32_t width, const uint32_t height) {
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(width, height);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2DArray> Texture2DArray::Create(uint32_t width, uint32_t height, uint32_t numTextures, uint32_t mipLevels) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2DArray>(width, height, numTextures, mipLevels);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2DArray> Texture2DArray::Create(const std::string* path, uint32_t numTextures, uint32_t mipLevels) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2DArray>(path, numTextures, mipLevels);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}
