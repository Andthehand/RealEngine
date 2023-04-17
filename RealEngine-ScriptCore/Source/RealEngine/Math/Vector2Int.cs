using System;
using System.Numerics;

namespace RealEngine {
	public struct Vector2Int {
		public int X, Y;

		public static Vector2Int Zero => new Vector2Int(0);

		public Vector2Int(Vector2 vector2) {
			X = (int)vector2.X;
			Y = (int)vector2.Y;
		}

		public Vector2Int(int scalar) {
			X = scalar;
			Y = scalar;
		}

		public Vector2Int(int x, int y) {
			X = x;
			Y = y;
		}

		public static Vector2Int operator +(Vector2Int a, Vector2Int b) {
			return new Vector2Int(a.X + b.X, a.Y + b.Y);
		}

		public static Vector2Int operator *(Vector2Int vector, int scalar) {
			return new Vector2Int(vector.X * scalar, vector.Y * scalar);
		}

		public float LengthSquared() {
			return X * X + Y * Y;
		}

		public float Length() {
			return (float)Math.Sqrt(LengthSquared());
		}

		public static bool operator ==(Vector2Int vec1, Vector2Int vec2) {
			return vec1.Equals(vec2);
		}

		public static bool operator !=(Vector2Int vec1, Vector2Int vec2) {
			return !vec1.Equals(vec2);
		}
	}
}
