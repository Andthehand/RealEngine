
using System;

namespace RealEngine {
	public struct Color {
		public float R, G, B, A;

		public static Color Zero => new Color(0.0f);

		public Color(float scalar) {
			R = scalar;
			G = scalar;
			B = scalar;
			A = scalar;
		}

		public Color(float x, float y, float z, float w) {
			R = x;
			G = y;
			B = z;
			A = w;
		}

		public Color(Vector2 xy, float z, float w) {
			R = xy.X;
			G = xy.Y;
			B = z;
			A = w;
		}

		public Color(Vector3 xyz, float w) {
			R = xyz.X;
			G = xyz.Y;
			B = xyz.Z;
			A = w;
		}

		public Color(Vector4 xyzw) {
			R = xyzw.X;
			G = xyzw.Y;
			B = xyzw.Z;
			A = xyzw.W;
		}

		public static Color operator +(Color a, Color b) {
			return new Color(a.R + b.R, a.G + b.G, a.B + b.B, a.A + b.A);
		}

		public static Color operator *(Color color, float scalar) {
			return new Color(color.R * scalar, color.G * scalar, color.B * scalar, color.A * scalar);
		}

		public static Color operator /(Color color, float scalar)
		{
			return new Color(color.R / scalar, color.G / scalar, color.B / scalar, color.A / scalar);
		}
	}
}
