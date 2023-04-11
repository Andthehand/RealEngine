namespace RealEngine {
	public class Input {
		public static bool IsKeyDown(KeyCode keycode) {
			return InternalCalls.Input_IsKeyDown(keycode);
		}

		public static Vector2 GetMousePosition() {
			InternalCalls.Input_GetMousePosition(out Vector2 mousePos);
			return mousePos;
		}
	}
}
