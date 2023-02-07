#include "repch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace RealEngine {
	static GLenum BufferTargetTypeToOpenGLBaseType(BufferTarget type) {
		switch (type) {
			case RealEngine::BufferTarget::None: break;
		}

		RE_CORE_ASSERT(false, "BufferTargetTypeToOpenGLBaseType Has not been implemented yet");
		//RE_CORE_ASSERT(false, "Unknown BufferTarget");
		return 0;
	}

	static GLenum BufferUsageTypeToOpenGLBaseType(BufferUsage type) {
		switch (type) {
			case RealEngine::BufferUsage::None: break;
			case RealEngine::BufferUsage::StreamDraw:  return	GL_STREAM_DRAW;
			case RealEngine::BufferUsage::StreamRead:  return	GL_STREAM_READ;
			case RealEngine::BufferUsage::StreamCopy:  return	GL_STREAM_COPY;
			case RealEngine::BufferUsage::StaticDraw:  return	GL_STATIC_DRAW;
			case RealEngine::BufferUsage::StaticRead:  return	GL_STATIC_READ;
			case RealEngine::BufferUsage::StaticCopy:  return	GL_STATIC_COPY;
			case RealEngine::BufferUsage::DynamicDraw: return	GL_DYNAMIC_DRAW;
			case RealEngine::BufferUsage::DynamicRead: return	GL_DYNAMIC_READ;
			case RealEngine::BufferUsage::DynamicCopy: return	GL_DYNAMIC_COPY;
		}

		RE_CORE_ASSERT(false, "Unknown BufferUsage");
		return 0;
	}

	static GLenum BufferModeTypeToOpenGLBaseType(BufferMode type) {
		switch (type) {
			case RealEngine::BufferMode::None: break;
			case RealEngine::BufferMode::Points: return					GL_POINTS;
			case RealEngine::BufferMode::LineStrip: return				GL_LINE_STRIP;
			case RealEngine::BufferMode::LineLoop: return				GL_LINE_LOOP;
			case RealEngine::BufferMode::Lines: return					GL_LINES;
			case RealEngine::BufferMode::LineStripAdjacency: return		GL_LINE_STRIP_ADJACENCY;
			case RealEngine::BufferMode::LinesAdjacency: return			GL_LINES_ADJACENCY;
			case RealEngine::BufferMode::TriangleStrip: return			GL_TRIANGLE_STRIP;
			case RealEngine::BufferMode::TriangleFan: return			GL_TRIANGLE_FAN;
			case RealEngine::BufferMode::Triangles: return				GL_TRIANGLES;
			case RealEngine::BufferMode::TriangleStripAdjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
			case RealEngine::BufferMode::TrianglesAdjacency: return		GL_TRIANGLES_ADJACENCY;
			case RealEngine::BufferMode::Patches: return				GL_PATCHES;
		}

		RE_CORE_ASSERT(false, "Unknown BufferMode");
		return 0;
	}

	//TODO: Move this to Texture or some other helper class
	static GLenum ColorFormatTypeToOpenGLBaseType(ColorFormat type) {
		switch (type) {
			case RealEngine::ColorFormat::None:  break;
			case RealEngine::ColorFormat::R8:	 return		GL_R8;
			case RealEngine::ColorFormat::R32F:	 return		GL_R32F;
			case RealEngine::ColorFormat::RG32F: return		GL_RG32F;
		}

		RE_CORE_ASSERT(false, "Unknown ColorFormat");
		return 0;
	}

	//------------------------------------
	// General Buffers -------------------
	//------------------------------------
	OpenGLBuffer::OpenGLBuffer(BufferTarget target, uint32_t size, const void* data, BufferUsage usage)
		: m_Target(BufferTargetTypeToOpenGLBaseType(target)) {
		RE_PROFILE_FUNCTION();

		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(m_Target, m_RendererID);
		glBufferData(m_Target, size, data, BufferUsageTypeToOpenGLBaseType(usage));
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
	OpenGLTextureBuffer::OpenGLTextureBuffer(uint32_t size, const void* data, BufferUsage usage, ColorFormat colorFormat) {
		RE_PROFILE_FUNCTION();

		//Create and load TBO
		glCreateBuffers(1, &m_BufferRendererID);
		glBindBuffer(GL_TEXTURE_BUFFER, m_BufferRendererID);
		glBufferData(GL_TEXTURE_BUFFER, size, data, BufferUsageTypeToOpenGLBaseType(usage));

		//Bind buffer to texture
		glGenTextures(1, &m_TextureRendererID);
		glBindTexture(GL_TEXTURE_BUFFER, m_TextureRendererID);
		glTexBuffer(GL_TEXTURE_BUFFER, ColorFormatTypeToOpenGLBaseType(colorFormat), m_BufferRendererID);

		glBindBuffer(GL_TEXTURE_BUFFER, 0);
		glBindTexture(GL_TEXTURE_BUFFER, 0);
	}

	OpenGLTextureBuffer::~OpenGLTextureBuffer() {
		RE_PROFILE_FUNCTION();

		glDeleteBuffers(1, &m_BufferRendererID);
	}

	void OpenGLTextureBuffer::Bind() const {
		RE_PROFILE_FUNCTION();

		glBindTexture(GL_TEXTURE_BUFFER, m_TextureRendererID);
	}

	void OpenGLTextureBuffer::Unbind() const {
		RE_PROFILE_FUNCTION();

		glBindTexture(GL_TEXTURE_BUFFER, 0);
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
