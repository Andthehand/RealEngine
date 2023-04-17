namespace RealEngine {
	public struct Vector3Int {
		public int X, Y, Z;

		public static Vector3Int Zero => new Vector3Int(0);

		public Vector3Int(Vector3 vector3) {
			X = (int)vector3.X;
			Y = (int)vector3.Y;
			Z = (int)vector3.Z;
		}

		public Vector3Int(int scalar) {
			X = scalar;
			Y = scalar;
			Z = scalar;
		}

		public Vector3Int(int x, int y, int z) {
			X = x;
			Y = y;
			Z = z;
		}

		public Vector3Int(Vector2Int xy, int z) {
			X = xy.X;
			Y = xy.Y;
			Z = z;
		}

		public Vector2Int XY {
			get => new Vector2Int(X, Y);
			set {
				X = value.X;
				Y = value.Y;
			}
		}

		public static Vector3Int operator +(Vector3Int a, Vector3Int b) {
			return new Vector3Int(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
		}

		public static Vector3Int operator *(Vector3Int vector, int scalar) {
			return new Vector3Int(vector.X * scalar, vector.Y * scalar, vector.Z * scalar);
		}

		public static bool operator ==(Vector3Int vec1, Vector3Int vec2) {
			return vec1.Equals(vec2);
		}

		public static bool operator !=(Vector3Int vec1, Vector3Int vec2) {
			return !vec1.Equals(vec2);
		}
	}
}
