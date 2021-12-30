#include "repch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace RealEngine {
	//------------------------------------
	// VertexBuffer -----------------------
	//------------------------------------
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* verticies, uint32_t size) {
		//Create and bind VBO to VAO
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ARRAY_BUFFER, size, verticies, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer(){
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	
	void OpenGLVertexBuffer::Unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//------------------------------------
	// IndexBuffer -----------------------
	//------------------------------------
	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) : m_Count(count) {
		//Create and bind EBO to VAO
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}