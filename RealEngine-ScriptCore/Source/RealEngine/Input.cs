namespace RealEngine {
	public class Input {
		public static bool IsKeyDown(KeyCode keycode) {
			return InternalCalls.Input_IsKeyDown(keycode);
		}

		public static bool IsKeyDown(MouseCode mousecode) {
			return InternalCalls.Input_IsMouseButtonPressed(mousecode);
		}

		public static Vector2 GetMousePosition() {
			InternalCalls.Input_GetMousePosition(out Vector2 mousePos);
			return mousePos;
		}
	}
}
