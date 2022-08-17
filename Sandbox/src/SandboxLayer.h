#pragma once
#include <RealEngine.h>

using namespace RealEngine;

class SandboxLayer : public Layer{
	virtual void OnUpdate(Timestep ts) override;
	virtual void OnImGuiRender() override;
};
