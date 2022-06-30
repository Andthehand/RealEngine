#pragma once
#include <glm/glm.hpp>

#include "RealEngine/Core/KeyCodes.h"
#include "RealEngine/Core/MouseCodes.h"

namespace RealEngine{

	class Input {
	public:
		static bool IsKeyPressed(KeyCode key);

		static bool IsMouseButtonPressed(MouseCode button);
		static glm::vec2 GetMousePosition();
		static float GetMouseX();
		static float GetMouseY();
	};
}
