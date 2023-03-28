using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RealEngine {
	public abstract class Component {
		public Entity Entity { get; internal set; }
	}

	public class SpriteRendererComponent : Component {
		public Color Color {
			get {
				InternalCalls.SpriteRendererComponent_GetColor(Entity.ID, out Color color);
				return color * 255.0f;
			}
			set {
				Color outColor = value / 255.0f;
				InternalCalls.SpriteRendererComponent_SetColor(Entity.ID, ref outColor);
			}
		}

		float TilingFactor {
			get => InternalCalls.SpriteRendererComponent_GetTilingFactor(Entity.ID);
			set => InternalCalls.SpriteRendererComponent_SetTilingFactor(Entity.ID, value);
		}
	}

	public class CircleRendererComponent : Component {
		public Color Color {
			get {
				InternalCalls.CircleRendererComponent_GetColor(Entity.ID, out Color color);
				return color * 255.0f;
			}
			set {
				Color outColor = value / 255.0f;
				InternalCalls.CircleRendererComponent_SetColor(Entity.ID, ref outColor);
			}
		}

		public float Thickness {
			get => InternalCalls.CircleRendererComponent_GetThickness(Entity.ID);
			set => InternalCalls.CircleRendererComponent_SetThickness(Entity.ID, value);
		}

		public float Fade {
			get => InternalCalls.CircleRendererComponent_GetFade(Entity.ID);
			set => InternalCalls.CircleRendererComponent_SetFade(Entity.ID, value);
		}
	}

	public class ScriptComponent : Component {
	}

		public class TransformComponent : Component {
	public Vector3 Translation {
			get {
				InternalCalls.TransformComponent_GetTranslation(Entity.ID, out Vector3 translation);
				return translation;
			}
			set {
				InternalCalls.TransformComponent_SetTranslation(Entity.ID, ref value);
			}
		}
	}

	public class Rigidbody2DComponent : Component {
		public enum BodyType { Static = 0, Dynamic, Kinematic }

		public Vector2 LinearVelocity {
			get {
				InternalCalls.Rigidbody2DComponent_GetLinearVelocity(Entity.ID, out Vector2 velocity);
				return velocity;
			}
		}

		public BodyType Type {
			get => InternalCalls.Rigidbody2DComponent_GetType(Entity.ID);
			set => InternalCalls.Rigidbody2DComponent_SetType(Entity.ID, value);
		}

		public void ApplyLinearImpulse(Vector2 impulse, Vector2 worldPosition, bool wake) {
			InternalCalls.Rigidbody2DComponent_ApplyLinearImpulse(Entity.ID, ref impulse, ref worldPosition, wake);
		}

		public void ApplyLinearImpulse(Vector2 impulse, bool wake) {
			InternalCalls.Rigidbody2DComponent_ApplyLinearImpulseToCenter(Entity.ID, ref impulse, wake);
		}
	}
}
