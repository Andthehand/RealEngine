#pragma once
#include <RealEngine.h>

#include "Bus.h"

class MainLayer : public RealEngine::Layer {
public:
	MainLayer();
	virtual void OnAttach() override;
	virtual void OnUpdate(RealEngine::Timestep ts);
	virtual void OnImGuiRender() override;

private:
	
};
