namespace RealEngine {
	public struct Vector4Int {
		public int X, Y, Z, W;

		public static Vector4Int Zero => new Vector4Int(0);

		public Vector4Int(Vector4 vector4) {
			X = (int)vector4.X;
			Y = (int)vector4.Y;
			Z = (int)vector4.Z;
			W = (int)vector4.W;
		}

		public Vector4Int(int scalar) {
			X = scalar;
			Y = scalar;
			Z = scalar;
			W = scalar;
		}

		public Vector4Int(int x, int y, int z, int w) {
			X = x;
			Y = y;
			Z = z;
			W = w;
		}

		public Vector4Int(Vector2Int xy, int z, int w) {
			X = xy.X;
			Y = xy.Y;
			Z = z;
			W = w;
		}

		public Vector4Int(Vector3Int xyz, int w) {
			X = xyz.X;
			Y = xyz.Y;
			Z = xyz.Z;
			W = w;
		}

		public Vector2Int XY {
			get => new Vector2Int(X, Y);
			set {
				X = value.X;
				Y = value.Y;
			}
		}

		public Vector3Int XYZ {
			get => new Vector3Int(X, Y, Z);
			set {
				X = value.X;
				Y = value.Y;
				Z = value.Z;
			}
		}

		public static Vector4Int operator +(Vector4Int a, Vector4Int b) {
			return new Vector4Int(a.X + b.X, a.Y + b.Y, a.Z + b.Z, a.W + b.W);
		}

		public static Vector4Int operator *(Vector4Int vector, int scalar) {
			return new Vector4Int(vector.X * scalar, vector.Y * scalar, vector.Z * scalar, vector.W * scalar);
		}

		public static Vector4Int operator /(Vector4Int vector, int scalar) {
			return new Vector4Int(vector.X / scalar, vector.Y / scalar, vector.Z / scalar, vector.W / scalar);
		}

		public static bool operator ==(Vector4Int vec1, Vector4Int vec2) {
			return vec1.Equals(vec2);
		}

		public static bool operator !=(Vector4Int vec1, Vector4Int vec2) {
			return !vec1.Equals(vec2);
		}
	}
}
