#pragma once

namespace RealEngine {
	typedef enum class KeyCode : uint16_t {
		// From glfw3.h
		Space = 32,
		Apostrophe = 39, /* ' */
		Comma = 44, /* , */
		Minus = 45, /* - */
		Period = 46, /* . */
		Slash = 47, /* / */

		D0 = 48, /* 0 */
		D1 = 49, /* 1 */
		D2 = 50, /* 2 */
		D3 = 51, /* 3 */
		D4 = 52, /* 4 */
		D5 = 53, /* 5 */
		D6 = 54, /* 6 */
		D7 = 55, /* 7 */
		D8 = 56, /* 8 */
		D9 = 57, /* 9 */

		Semicolon = 59, /* ; */
		Equal = 61, /* = */

		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,

		LeftBracket = 91,  /* [ */
		Backslash = 92,  /* \ */
		RightBracket = 93,  /* ] */
		GraveAccent = 96,  /* ` */

		World1 = 161, /* non-US #1 */
		World2 = 162, /* non-US #2 */

		/* Function keys */
		Escape = 256,
		Enter = 257,
		Tab = 258,
		Backspace = 259,
		Insert = 260,
		Delete = 261,
		Right = 262,
		Left = 263,
		Down = 264,
		Up = 265,
		PageUp = 266,
		PageDown = 267,
		Home = 268,
		End = 269,
		CapsLock = 280,
		ScrollLock = 281,
		NumLock = 282,
		PrintScreen = 283,
		Pause = 284,
		F1 = 290,
		F2 = 291,
		F3 = 292,
		F4 = 293,
		F5 = 294,
		F6 = 295,
		F7 = 296,
		F8 = 297,
		F9 = 298,
		F10 = 299,
		F11 = 300,
		F12 = 301,
		F13 = 302,
		F14 = 303,
		F15 = 304,
		F16 = 305,
		F17 = 306,
		F18 = 307,
		F19 = 308,
		F20 = 309,
		F21 = 310,
		F22 = 311,
		F23 = 312,
		F24 = 313,
		F25 = 314,

		/* Keypad */
		KP0 = 320,
		KP1 = 321,
		KP2 = 322,
		KP3 = 323,
		KP4 = 324,
		KP5 = 325,
		KP6 = 326,
		KP7 = 327,
		KP8 = 328,
		KP9 = 329,
		KPDecimal = 330,
		KPDivide = 331,
		KPMultiply = 332,
		KPSubtract = 333,
		KPAdd = 334,
		KPEnter = 335,
		KPEqual = 336,

		LeftShift = 340,
		LeftControl = 341,
		LeftAlt = 342,
		LeftSuper = 343,
		RightShift = 344,
		RightControl = 345,
		RightAlt = 346,
		RightSuper = 347,
		Menu = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode) {
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define RE_KEY_SPACE           ::RealEngine::Key::Space
#define RE_KEY_APOSTROPHE      ::RealEngine::Key::Apostrophe    /* ' */
#define RE_KEY_COMMA           ::RealEngine::Key::Comma         /* , */
#define RE_KEY_MINUS           ::RealEngine::Key::Minus         /* - */
#define RE_KEY_PERIOD          ::RealEngine::Key::Period        /* . */
#define RE_KEY_SLASH           ::RealEngine::Key::Slash         /* / */
#define RE_KEY_0               ::RealEngine::Key::D0
#define RE_KEY_1               ::RealEngine::Key::D1
#define RE_KEY_2               ::RealEngine::Key::D2
#define RE_KEY_3               ::RealEngine::Key::D3
#define RE_KEY_4               ::RealEngine::Key::D4
#define RE_KEY_5               ::RealEngine::Key::D5
#define RE_KEY_6               ::RealEngine::Key::D6
#define RE_KEY_7               ::RealEngine::Key::D7
#define RE_KEY_8               ::RealEngine::Key::D8
#define RE_KEY_9               ::RealEngine::Key::D9
#define RE_KEY_SEMICOLON       ::RealEngine::Key::Semicolon     /* ; */
#define RE_KEY_EQUAL           ::RealEngine::Key::Equal         /* = */
#define RE_KEY_A               ::RealEngine::Key::A
#define RE_KEY_B               ::RealEngine::Key::B
#define RE_KEY_C               ::RealEngine::Key::C
#define RE_KEY_D               ::RealEngine::Key::D
#define RE_KEY_E               ::RealEngine::Key::E
#define RE_KEY_F               ::RealEngine::Key::F
#define RE_KEY_G               ::RealEngine::Key::G
#define RE_KEY_H               ::RealEngine::Key::H
#define RE_KEY_I               ::RealEngine::Key::I
#define RE_KEY_J               ::RealEngine::Key::J
#define RE_KEY_K               ::RealEngine::Key::K
#define RE_KEY_L               ::RealEngine::Key::L
#define RE_KEY_M               ::RealEngine::Key::M
#define RE_KEY_N               ::RealEngine::Key::N
#define RE_KEY_O               ::RealEngine::Key::O
#define RE_KEY_P               ::RealEngine::Key::P
#define RE_KEY_Q               ::RealEngine::Key::Q
#define RE_KEY_R               ::RealEngine::Key::R
#define RE_KEY_S               ::RealEngine::Key::S
#define RE_KEY_T               ::RealEngine::Key::T
#define RE_KEY_U               ::RealEngine::Key::U
#define RE_KEY_V               ::RealEngine::Key::V
#define RE_KEY_W               ::RealEngine::Key::W
#define RE_KEY_X               ::RealEngine::Key::X
#define RE_KEY_Y               ::RealEngine::Key::Y
#define RE_KEY_Z               ::RealEngine::Key::Z
#define RE_KEY_LEFT_BRACKET    ::RealEngine::Key::LeftBracket   /* [ */
#define RE_KEY_BACKSLASH       ::RealEngine::Key::Backslash     /* \ */
#define RE_KEY_RIGHT_BRACKET   ::RealEngine::Key::RightBracket  /* ] */
#define RE_KEY_GRAVE_ACCENT    ::RealEngine::Key::GraveAccent   /* ` */
#define RE_KEY_WORLD_1         ::RealEngine::Key::World1        /* non-US #1 */
#define RE_KEY_WORLD_2         ::RealEngine::Key::World2        /* non-US #2 */

/* Function keys */
#define RE_KEY_ESCAPE          ::RealEngine::Key::Escape
#define RE_KEY_ENTER           ::RealEngine::Key::Enter
#define RE_KEY_TAB             ::RealEngine::Key::Tab
#define RE_KEY_BACKSPACE       ::RealEngine::Key::Backspace
#define RE_KEY_INSERT          ::RealEngine::Key::Insert
#define RE_KEY_DELETE          ::RealEngine::Key::Delete
#define RE_KEY_RIGHT           ::RealEngine::Key::Right
#define RE_KEY_LEFT            ::RealEngine::Key::Left
#define RE_KEY_DOWN            ::RealEngine::Key::Down
#define RE_KEY_UP              ::RealEngine::Key::Up
#define RE_KEY_PAGE_UP         ::RealEngine::Key::PageUp
#define RE_KEY_PAGE_DOWN       ::RealEngine::Key::PageDown
#define RE_KEY_HOME            ::RealEngine::Key::Home
#define RE_KEY_END             ::RealEngine::Key::End
#define RE_KEY_CAPS_LOCK       ::RealEngine::Key::CapsLock
#define RE_KEY_SCROLL_LOCK     ::RealEngine::Key::ScrollLock
#define RE_KEY_NUM_LOCK        ::RealEngine::Key::NumLock
#define RE_KEY_PRINT_SCREEN    ::RealEngine::Key::PrintScreen
#define RE_KEY_PAUSE           ::RealEngine::Key::Pause
#define RE_KEY_F1              ::RealEngine::Key::F1
#define RE_KEY_F2              ::RealEngine::Key::F2
#define RE_KEY_F3              ::RealEngine::Key::F3
#define RE_KEY_F4              ::RealEngine::Key::F4
#define RE_KEY_F5              ::RealEngine::Key::F5
#define RE_KEY_F6              ::RealEngine::Key::F6
#define RE_KEY_F7              ::RealEngine::Key::F7
#define RE_KEY_F8              ::RealEngine::Key::F8
#define RE_KEY_F9              ::RealEngine::Key::F9
#define RE_KEY_F10             ::RealEngine::Key::F10
#define RE_KEY_F11             ::RealEngine::Key::F11
#define RE_KEY_F12             ::RealEngine::Key::F12
#define RE_KEY_F13             ::RealEngine::Key::F13
#define RE_KEY_F14             ::RealEngine::Key::F14
#define RE_KEY_F15             ::RealEngine::Key::F15
#define RE_KEY_F16             ::RealEngine::Key::F16
#define RE_KEY_F17             ::RealEngine::Key::F17
#define RE_KEY_F18             ::RealEngine::Key::F18
#define RE_KEY_F19             ::RealEngine::Key::F19
#define RE_KEY_F20             ::RealEngine::Key::F20
#define RE_KEY_F21             ::RealEngine::Key::F21
#define RE_KEY_F22             ::RealEngine::Key::F22
#define RE_KEY_F23             ::RealEngine::Key::F23
#define RE_KEY_F24             ::RealEngine::Key::F24
#define RE_KEY_F25             ::RealEngine::Key::F25

/* Keypad */
#define RE_KEY_KP_0            ::RealEngine::Key::KP0
#define RE_KEY_KP_1            ::RealEngine::Key::KP1
#define RE_KEY_KP_2            ::RealEngine::Key::KP2
#define RE_KEY_KP_3            ::RealEngine::Key::KP3
#define RE_KEY_KP_4            ::RealEngine::Key::KP4
#define RE_KEY_KP_5            ::RealEngine::Key::KP5
#define RE_KEY_KP_6            ::RealEngine::Key::KP6
#define RE_KEY_KP_7            ::RealEngine::Key::KP7
#define RE_KEY_KP_8            ::RealEngine::Key::KP8
#define RE_KEY_KP_9            ::RealEngine::Key::KP9
#define RE_KEY_KP_DECIMAL      ::RealEngine::Key::KPDecimal
#define RE_KEY_KP_DIVIDE       ::RealEngine::Key::KPDivide
#define RE_KEY_KP_MULTIPLY     ::RealEngine::Key::KPMultiply
#define RE_KEY_KP_SUBTRACT     ::RealEngine::Key::KPSubtract
#define RE_KEY_KP_ADD          ::RealEngine::Key::KPAdd
#define RE_KEY_KP_ENTER        ::RealEngine::Key::KPEnter
#define RE_KEY_KP_EQUAL        ::RealEngine::Key::KPEqual

#define RE_KEY_LEFT_SHIFT      ::RealEngine::Key::LeftShift
#define RE_KEY_LEFT_CONTROL    ::RealEngine::Key::LeftControl
#define RE_KEY_LEFT_ALT        ::RealEngine::Key::LeftAlt
#define RE_KEY_LEFT_SUPER      ::RealEngine::Key::LeftSuper
#define RE_KEY_RIGHT_SHIFT     ::RealEngine::Key::RightShift
#define RE_KEY_RIGHT_CONTROL   ::RealEngine::Key::RightControl
#define RE_KEY_RIGHT_ALT       ::RealEngine::Key::RightAlt
#define RE_KEY_RIGHT_SUPER     ::RealEngine::Key::RightSuper
#define RE_KEY_MENU            ::RealEngine::Key::Menu
