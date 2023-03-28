using System;

namespace RealEngine {
	public static class Logging {
		public static void Log(string message) {
			InternalCalls.NativeLog(message);
		}
		public static void Log(ulong parameter, string format = "{0}") {
			InternalCalls.NativeLog_ULong(parameter, format);
		}
		public static void Log(ref Vector2 parameter, string format = "{0}") {
			InternalCalls.NativeLog_Vector2(ref parameter, format);
		}
		public static void Log(ref Vector3 parameter, string format = "{0}") {
			InternalCalls.NativeLog_Vector3(ref parameter, format);
		}
		public static void Log(ref Vector4 parameter, string format = "{0}") {
			InternalCalls.NativeLog_Vector4(ref parameter, format);
		}
	}
}
