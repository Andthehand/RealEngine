#include "repch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace RealEngine {
	//------------------------------------
	// General Buffers -------------------
	//------------------------------------
	OpenGLBuffer::OpenGLBuffer(uint32_t target, uint32_t size, const void* data, uint32_t usage)
		: m_Target(target) {
		RE_PROFILE_FUNCTION();

		//Create and bind VBO to VAO
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(target, m_RendererID);
		glBufferData(target, size, data, usage);
	}

	OpenGLBuffer::~OpenGLBuffer() {
		RE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLBuffer::Bind() const {
		RE_PROFILE_FUNCTION();

		glBindBuffer(m_Target, m_RendererID);
	}

	void OpenGLBuffer::Unbind() const {
		RE_PROFILE_FUNCTION();

		glBindBuffer(m_Target, 0);
	}

	void OpenGLBuffer::SetData(const void* data, uint32_t size) {
		glBindBuffer(m_Target, m_RendererID);
		glBufferSubData(m_Target, 0, size, data);
	}


	//------------------------------------
	// TextureBuffer ----------------------
	//------------------------------------
	OpenGLTextureBuffer::OpenGLTextureBuffer(uint32_t size, const void* data, uint32_t usage, uint32_t internalFormat)
		: m_Usage(usage), m_InternalFormat(internalFormat) {
		RE_PROFILE_FUNCTION();

		//Create and bind VBO to VAO
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_TEXTURE_BUFFER, m_RendererID);
		glBufferData(GL_TEXTURE_BUFFER, size, data, usage);
	}

	OpenGLTextureBuffer::~OpenGLTextureBuffer() {
		RE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLTextureBuffer::Bind() const {
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_TEXTURE_BUFFER, m_RendererID);
	}

	void OpenGLTextureBuffer::BindToTexture(const Texture& texture) const {
		texture.Bind();
		glTexBuffer(GL_TEXTURE_BUFFER, m_InternalFormat, m_RendererID);
	}

	void OpenGLTextureBuffer::Unbind() const {
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_TEXTURE_BUFFER, 0);
	}

	void OpenGLTextureBuffer::SetData(const void* data, uint32_t size) {
		glBufferData(GL_TEXTURE_BUFFER, size, data, m_Usage);
	}

	//------------------------------------
	// VertexBuffer ----------------------
	//------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
		RE_PROFILE_FUNCTION();

		//Create and bind VBO to VAO
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(const float* verticies, uint32_t size) {
		RE_PROFILE_FUNCTION();

		//Create and bind VBO to VAO
		glCreateBuffers(1, &m_RendererID);

		// GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
		// Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO state. 
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer(){
		RE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const {
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	
	void OpenGLVertexBuffer::Unbind() const {
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	//------------------------------------
	// IndexBuffer -----------------------
	//------------------------------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t count) {
		RE_PROFILE_FUNCTION();

		//Create and bind EBO to VAO
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), nullptr, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count) : m_Count(count) {
		RE_PROFILE_FUNCTION();

		//Create and bind EBO to VAO
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		RE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::SetData(const uint32_t* data, uint32_t count) {
		m_Count = count;
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, count * sizeof(uint32_t), data);
	}

	void OpenGLIndexBuffer::Bind() const {
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		RE_PROFILE_FUNCTION();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
