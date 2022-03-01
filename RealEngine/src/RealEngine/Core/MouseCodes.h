#pragma once

namespace RealEngine {
	typedef enum class MouseCode : uint16_t {
		// From glfw3.h
		Button0 = 0,
		Button1 = 1,
		Button2 = 2,
		Button3 = 3,
		Button4 = 4,
		Button5 = 5,
		Button6 = 6,
		Button7 = 7,

		ButtonLast = Button7,
		ButtonLeft = Button0,
		ButtonRight = Button1,
		ButtonMiddle = Button2
	} Mouse;

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode) {
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define RE_MOUSE_BUTTON_0      ::RealEngine::Mouse::Button0
#define RE_MOUSE_BUTTON_1      ::RealEngine::Mouse::Button1
#define RE_MOUSE_BUTTON_2      ::RealEngine::Mouse::Button2
#define RE_MOUSE_BUTTON_3      ::RealEngine::Mouse::Button3
#define RE_MOUSE_BUTTON_4      ::RealEngine::Mouse::Button4
#define RE_MOUSE_BUTTON_5      ::RealEngine::Mouse::Button5
#define RE_MOUSE_BUTTON_6      ::RealEngine::Mouse::Button6
#define RE_MOUSE_BUTTON_7      ::RealEngine::Mouse::Button7
#define RE_MOUSE_BUTTON_LAST   ::RealEngine::Mouse::ButtonLast
#define RE_MOUSE_BUTTON_LEFT   ::RealEngine::Mouse::ButtonLeft
#define RE_MOUSE_BUTTON_RIGHT  ::RealEngine::Mouse::ButtonRight
#define RE_MOUSE_BUTTON_MIDDLE ::RealEngine::Mouse::ButtonMiddle
