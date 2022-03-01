#include "repch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace RealEngine {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case RealEngine::ShaderDataType::None:    return GL_FLOAT;
			case RealEngine::ShaderDataType::Float:	  return GL_FLOAT;
			case RealEngine::ShaderDataType::Float2:  return GL_FLOAT;
			case RealEngine::ShaderDataType::Float3:  return GL_FLOAT;
			case RealEngine::ShaderDataType::Float4:  return GL_FLOAT;
			case RealEngine::ShaderDataType::Mat3:	  return GL_FLOAT;
			case RealEngine::ShaderDataType::Mat4:	  return GL_FLOAT;
			case RealEngine::ShaderDataType::Int:	  return GL_INT;
			case RealEngine::ShaderDataType::Int2:	  return GL_INT;
			case RealEngine::ShaderDataType::Int3:	  return GL_INT;
			case RealEngine::ShaderDataType::Int4:	  return GL_INT;
			case RealEngine::ShaderDataType::Bool:    return GL_BOOL;
		}

		RE_CORE_ASSERT(false, "Unknown ShaderDataType");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		RE_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		RE_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}
	
	void OpenGLVertexArray::Bind() const {
		RE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}
	
	void OpenGLVertexArray::Unbind() const {
		RE_PROFILE_FUNCTION();

		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		RE_PROFILE_FUNCTION();

		RE_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		//Loops through the elements to add to the VBO such as a_Position and a_Color
		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout) {
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				(const void*)element.Offset);
			m_VertexBufferIndex++;
		}
		
		m_VertexBuffers.push_back(vertexBuffer);
	}
	
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		RE_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();

		m_IndexBuffer = indexBuffer;
	}
}