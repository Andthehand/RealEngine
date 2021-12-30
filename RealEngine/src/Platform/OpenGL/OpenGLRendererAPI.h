#pragma once

#include "RealEngine/Renderer/RendererAPI.h"

namespace RealEngine {

	class OpenGLRendererAPI : public RendererAPI{
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}