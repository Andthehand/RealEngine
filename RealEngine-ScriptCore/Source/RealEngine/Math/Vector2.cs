using System;
using System.Numerics;

namespace RealEngine {
	public struct Vector2 {
		public float X, Y;

		public static Vector2 Zero => new Vector2(0.0f);

		public Vector2(Vector2Int vector2Int) {
			X = (float)vector2Int.X;
			Y = (float)vector2Int.Y;
		}

		public Vector2(float scalar) {
			X = scalar;
			Y = scalar;
		}

		public Vector2(float x, float y) {
			X = x;
			Y = y;
		}

		public static Vector2 operator +(Vector2 a, Vector2 b) {
			return new Vector2(a.X + b.X, a.Y + b.Y);
		}

		public static Vector2 operator *(Vector2 vector, float scalar) {
			return new Vector2(vector.X * scalar, vector.Y * scalar);
		}

		public float LengthSquared() {
			return X * X + Y * Y;
		}

		public float Length() {
			return (float)Math.Sqrt(LengthSquared());
		}

		public static bool operator ==(Vector2 vec1, Vector2 vec2) {
			return vec1.Equals(vec2);
		}

		public static bool operator !=(Vector2 vec1, Vector2 vec2) {
			return !vec1.Equals(vec2);
		}
	}
}
