#include <RealEngine.h>

#include "imgui/imgui.h"

class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer() : Layer("Example") {
	
	}

	void OnUpdate() override {

	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
	}

	void OnEvent(RealEngine::Event& event) override {
		if (event.GetEventType() == RealEngine::EventType::KeyPressed) {
			RealEngine::KeyPressedEvent& e = (RealEngine::KeyPressedEvent&)event;
			//RE_TRACE("{0}", (char)e.GetKeyCode());
		}
	}

};

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {

	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}