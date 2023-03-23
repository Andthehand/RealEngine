#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace RealEngine {
	class RealEngineEditor : public Application {
	public:
		RealEngineEditor(const RealEngine ::ApplicationSpecification& specification)
			: Application(specification) {
			PushLayer(new EditorLayer());
		}

		~RealEngineEditor() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		ApplicationSpecification spec;
		spec.Name = "RealEngine Editor";
		spec.CommandLineArgs = args;

		return new RealEngineEditor(spec);
	}
}
