#pragma once

#include "RealEngine/Renderer/Buffer.h"

namespace RealEngine {
	//------------------------------------
	// General Buffer --------------------
	//------------------------------------
	class OpenGLBuffer : public Buffer {
	public:
		OpenGLBuffer(uint32_t target, uint32_t size, const void* data, uint32_t usage);
		virtual ~OpenGLBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Target;
	};

	//------------------------------------
	// Texture Buffer --------------------
	//------------------------------------
	class OpenGLTextureBuffer : public TextureBuffer {
	public:
		OpenGLTextureBuffer(uint32_t size, const void* data, uint32_t usage, uint32_t internalFormat);
		virtual ~OpenGLTextureBuffer();

		virtual uint32_t GetRenderID() const override { return m_TextureRendererID; }

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t m_BufferRendererID, m_TextureRendererID;
	};

	//------------------------------------
	// VertexBuffer ----------------------
	//------------------------------------
	class OpenGLVertexBuffer : public VertexBuffer {
	public:
		OpenGLVertexBuffer(uint32_t size);
		OpenGLVertexBuffer(const float* verticies, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; }
	private:
		uint32_t m_RendererID;
		BufferLayout m_Layout;
	};

	//------------------------------------
	// IndexBuffer -----------------------
	//------------------------------------
	class OpenGLIndexBuffer : public IndexBuffer {
	public:
		OpenGLIndexBuffer(uint32_t count);
		OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual uint32_t GetCount() const override { return m_Count; }

		virtual void SetData(const uint32_t* data, uint32_t count) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;
	private:
		uint32_t m_RendererID;
		uint32_t m_Count;
	};
}
