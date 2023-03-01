#pragma once

#include "RendererAPI.h"

namespace RealEngine {
	class RenderCommand {
	public:
		inline static void Init() {
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetWireFrame(bool enabled) {
			s_RendererAPI->SetWireFrame(enabled);
		}

		inline static void SetClearColor(const glm::vec4& color) {
			s_RendererAPI->SetClearColor(color);
		}
		
		inline static void Clear() {
			s_RendererAPI->Clear();
		};

		inline static void DrawArrays(const Ref<VertexArray>& vertexArray, uint32_t count, BufferMode mode = BufferMode::Triangles, uint32_t start = 0) {
			s_RendererAPI->DrawArrays(vertexArray, count, mode, start);
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0) {
			s_RendererAPI->DrawIndexed(vertexArray, indexCount);
		}

		inline static void DrawArraysInstanced(const Ref<VertexArray>& vertexArray, BufferMode mode, uint32_t verticeCount, uint32_t instanceCount, uint32_t start = 0) {
			s_RendererAPI->DrawArraysInstanced(vertexArray, mode, verticeCount, instanceCount, start);
		}

		inline static void DrawIndexedInstanced(const Ref<VertexArray>& vertexArray, uint32_t instanceCount, uint32_t indexCount = 0) {
			s_RendererAPI->DrawIndexedInstanced(vertexArray, instanceCount, indexCount);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}
