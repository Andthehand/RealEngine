#include "repch.h"
#include "Buffer.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace RealEngine {
	Ref<Buffer> Buffer::Create(BufferTarget target, uint32_t size, const void* data, BufferUsage usage) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLBuffer>(target, size, data, usage);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<TextureBuffer> TextureBuffer::Create(uint32_t size, const void* data, BufferUsage usage, ColorFormat colorFormat) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTextureBuffer>(size, data, usage, colorFormat);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(const float* verticies, uint32_t size) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(verticies, size);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t count)
	{
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(count);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(const uint32_t* indices, uint32_t count) {
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None:    RE_CORE_ASSERT(false, "RenderAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		RE_CORE_ASSERT(false, "Unkown RendererAPI!");
		return nullptr;
	}
}
