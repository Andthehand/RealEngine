	#include "repch.h"
#include "OpenGLRendererAPI.h"

#include <glad/glad.h>

#include "OpenGLConstants.h"

namespace RealEngine {

	void OpenGLMessageCallback (
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity) {
			case GL_DEBUG_SEVERITY_HIGH:         RE_CORE_CRITICAL(message); return;
			case GL_DEBUG_SEVERITY_MEDIUM:       RE_CORE_ERROR(message); return;
			case GL_DEBUG_SEVERITY_LOW:          RE_CORE_WARN(message); return;
			case GL_DEBUG_SEVERITY_NOTIFICATION: RE_CORE_TRACE(message); return;
		}

		RE_CORE_ASSERT(false, "Unknown severity level!");
	}

	void OpenGLRendererAPI::Init() {
		RE_PROFILE_FUNCTION();

	#ifdef RE_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
	#endif

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH);

		//MSAA
		glEnable(GL_MULTISAMPLE);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetWireFrame(bool enabled) {
		glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
	}
	
	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t count, BufferMode mode, uint32_t start) {
		glDrawArrays(BufferModeTypeToOpenGLBaseType(mode), start, count);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) {
		vertexArray->Bind(); 
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	void OpenGLRendererAPI::DrawArraysInstanced(const Ref<VertexArray>& vertexArray, BufferMode mode, uint32_t verticeCount, uint32_t instanceCount, uint32_t start) {
		glDrawArraysInstanced(BufferModeTypeToOpenGLBaseType(mode), start, verticeCount, instanceCount);
	}

	void OpenGLRendererAPI::DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instanceCount, uint32_t indexCount) {
		uint32_t count = indexCount ? indexCount : vertexArray->GetIndexBuffer()->GetCount();
		glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, instanceCount);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) {
		vertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetLineWidth(float width) {
		glLineWidth(width);
	}
}
