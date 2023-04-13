
using System;

namespace RealEngine {
	public struct Color {
		public float R, G, B, A;

		public static Color Zero => new Color(0.0f);

		public Color(float scalar) {
			R = scalar;
			G = scalar;
			B = scalar;
			A = 255.0f;
		}

		public Color(float r, float g, float b, float a) {
			R = r;
			G = g;
			B = b;
			A = a;
		}

		public Color(Vector2 rg, float b, float a) {
			R = rg.X;
			G = rg.Y;
			B = b;
			A = a;
		}

		public Color(Vector3 rgb, float a) {
			R = rgb.X;
			G = rgb.Y;
			B = rgb.Z;
			A = a;
		}

		public Color(Vector4 rgba) {
			R = rgba.X;
			G = rgba.Y;
			B = rgba.Z;
			A = rgba.W;
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
