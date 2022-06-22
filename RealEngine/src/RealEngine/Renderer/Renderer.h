#pragma once

#include "RenderCommand.h"

#include <RealEngine/Renderer/Shader.h>

#include "OrthographicCamera.h"

#include "RealEngine/Renderer/OrthographicCamera.h"

#include "RealEngine/Renderer/Texture.h"
#include "RealEngine/Renderer/SubTexture2D.h"

namespace RealEngine {
	class Renderer {
	public:
		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); };

		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Flush();

		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
	
		//Stats
		struct Statistics {
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static void ResetStats();
		static Statistics GetStats();
	private:
		static void FlushAndReset();
	};
}