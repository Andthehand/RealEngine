#include <RealEngine.h>
 
class ExampleLayer : public RealEngine::Layer {
public:
	ExampleLayer() : Layer("Example") {
	}

	void OnUpdate() override {
		//RE_INFO("ExampleLayer::Update");
	}

	void OnEvent(RealEngine::Event& event) override {
		if (event.GetEventType() == RealEngine::EventType::KeyPressed) {
			RealEngine::KeyPressedEvent& e = (RealEngine::KeyPressedEvent&)event;
			RE_TRACE("{0}", (char)e.GetKeyCode());
		}
		//RE_TRACE("{0}", event);
	}
};

class Sandbox : public RealEngine::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new RealEngine::ImGuiLayer());
	}

	~Sandbox() {

	}
};

RealEngine::Application* RealEngine::CreateApplication() {
	return new Sandbox();
}