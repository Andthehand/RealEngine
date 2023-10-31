#pragma once
#include <RealEngine.h>

#include "VisualShader/ShaderCreatePanel.h"

namespace RealEngine {
	class MainLayer : public Layer {
	public:
		MainLayer();
		virtual void OnImGuiRender() override;

	private:
		ShaderCreatePanel m_ShaderCreatePanel;
	};
}
