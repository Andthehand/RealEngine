#include "repch.h"
#include "RealEngine/Core/Input.h"

#ifdef RE_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsInput.h"
#endif

namespace RealEngine {
	Scope<Input> Input::s_Instance = Input::Create();

	Scope<Input> Input::Create() {
	#ifdef RE_PLATFORM_WINDOWS
		return CreateScope<WindowsInput>();
	#else
		RE_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
	#endif
	}
}