#include <RealEngine.h>
#include <RealEngine/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace RealEngine {
	class RealEngineEditor : public Application {
	public:
		RealEngineEditor(ApplicationCommandLineArgs args)
			: Application("RealEngine Editor", args) {
			PushLayer(new EditorLayer());
		}

		~RealEngineEditor() {

		}
	};

	Application* CreateApplication(ApplicationCommandLineArgs args) {
		return new RealEngineEditor(args);
	}
}
