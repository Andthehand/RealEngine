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
			case RealEngine::ShaderDataType::UInt:	  return GL_UNSIGNED_INT;
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
			switch (element.Type) {
			case ShaderDataType::UInt: {
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset);
				//This is for instancing
				glVertexAttribDivisor(m_VertexBufferIndex, element.InstancedDivisor);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool: {
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.Offset);
				//This is for instancing
				glVertexAttribDivisor(m_VertexBufferIndex, element.InstancedDivisor);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4: {
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; i++) {
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					//This is for instancing
					glVertexAttribDivisor(m_VertexBufferIndex, element.InstancedDivisor);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				RE_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
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
