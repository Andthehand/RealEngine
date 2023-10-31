#pragma once
#include <RealEngine.h>

class MainLayer : public RealEngine::Layer {
public:
	MainLayer();
	virtual void OnImGuiRender() override;

private:
};
