#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace RealEngine {
	class RealEngineEditor : public Application {
	public:
		RealEngineEditor() 
			: Application("RealEngine Editor") {
			//PushLayer(new ExampleLayer);
			PushLayer(new EditorLayer());
		}

		~RealEngineEditor() {

		}
	};

	Application* CreateApplication() {
		return new RealEngineEditor();
	}
}