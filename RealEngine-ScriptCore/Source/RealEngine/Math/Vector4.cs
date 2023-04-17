﻿namespace RealEngine {
	public struct Vector4 {
		public float X, Y, Z, W;

		public static Vector4 Zero => new Vector4(0.0f);

		public Vector4(Vector4Int vector4Int) {
			X = (float)vector4Int.X;
			Y = (float)vector4Int.Y;
			Z = (float)vector4Int.Z;
			W = (float)vector4Int.W;
		}

		public Vector4(float scalar) {
			X = scalar;
			Y = scalar;
			Z = scalar;
			W = scalar;
		}

		public Vector4(float x, float y, float z, float w) {
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		public Vector4(Vector2 xy, float z, float w) {
			X = xy.X;
			Y = xy.Y;
			Z = z;
			W = w;
		}

		public Vector4(Vector3 xyz, float w) {
			X = xyz.X;
			Y = xyz.Y;
			Z = xyz.Z;
			W = w;
		}

		public Vector2 XY {
			get => new Vector2(X, Y);
			set {
				X = value.X;
				Y = value.Y;
			}
		}

		public Vector3 XYZ {
			get => new Vector3(X, Y, Z);
			set {
				X = value.X;
				Y = value.Y;
				Z = value.Z;
			}
		}

		public static Vector4 operator +(Vector4 a, Vector4 b) {
			return new Vector4(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
		}

		public static Vector4 operator *(Vector4 vector, float scalar) {
			return new Vector4(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
		}

		public static Vector4 operator /(Vector4 vector, float scalar) {
			return new Vector4(vector.X / scalar, vector.Y / scalar, vector.Z / scalar, vector.W / scalar);
		}

		public static bool operator ==(Vector4 vec1, Vector4 vec2) {
			return vec1.Equals(vec2);
		}

		public static bool operator !=(Vector4 vec1, Vector4 vec2) {
			return !vec1.Equals(vec2);
		}
	}
}
