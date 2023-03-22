#pragma once

#include "RealEngine/Renderer/RendererAPI.h"

namespace RealEngine {

	class OpenGLRendererAPI : public RendererAPI{
		virtual void Init() override;
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetWireFrame(bool enabled) override;

		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		
		virtual void DrawArrays(const Ref<VertexArray>&vertexArray, uint32_t count, BufferMode mode = BufferMode::Triangles, uint32_t start = 0) override;
		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) override;
	
		virtual void DrawArraysInstanced(const Ref<VertexArray>& vertexArray, BufferMode mode, uint32_t verticeCount, uint32_t instanceCount, uint32_t start = 0) override;
		virtual void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instanceCount, uint32_t indexCount = 0) override;
	
		virtual void DrawLines(const Ref<VertexArray>& vertexArray, uint32_t vertexCount) override;

		virtual void SetLineWidth(float width) override;
	};
}
