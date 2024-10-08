﻿using System;
using System.Collections.Generic;
using System.Diagnostics.Contracts;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

using RealEngine;

namespace Sandbox {
	public class Player : Entity {
		private TransformComponent m_Transform;
		private Rigidbody2DComponent m_Rigidbody;
		private SpriteRendererComponent m_RenderComponent;

		public float Speed = 2.0f;
		public float Time = 0.0f;

		public Color ChangeEntityColor = new Color(255.0f);
		public Color EntityColor = new Color(255.0f);

		void OnCreate() {
			m_Transform = GetComponent<TransformComponent>();
			m_Rigidbody = GetComponent<Rigidbody2DComponent>();
			m_RenderComponent = GetComponent<SpriteRendererComponent>();

			EntityColor = m_RenderComponent.Color;
		}

		void OnUpdate(float ts) {
			// Console.WriteLine($"Player.OnUpdate: {ts}");
			Time += ts;

			Vector3 velocity = Vector3.Zero;

			if (Input.IsKeyDown(KeyCode.W))
				velocity.Y = 1.0f;
			else if (Input.IsKeyDown(KeyCode.S))
				velocity.Y = -1.0f;

			if (Input.IsKeyDown(KeyCode.A))
				velocity.X = -1.0f;
			else if (Input.IsKeyDown(KeyCode.D))
				velocity.X = 1.0f;

			Entity cameraEntity = FindEntityByName("Camera");
			if (cameraEntity != null) {
				Camera camera = cameraEntity.As<Camera>();

				if (Input.IsKeyDown(KeyCode.Q))
					camera.DistanceFromPlayer += Speed * 2.0f * ts;
				else if (Input.IsKeyDown(KeyCode.E))
					camera.DistanceFromPlayer -= Speed * 2.0f * ts;
			}

			velocity *= Speed * ts;

			m_Rigidbody.ApplyLinearImpulse(velocity.XY, true);

			//Vector3 translation = m_Transform.Translation;
			//translation += velocity * ts;
			//m_Transform.Translation = translation;
		}

		void OnBeginContact() {
			m_RenderComponent.Color = ChangeEntityColor;
		}

		void OnEndContact() {
			m_RenderComponent.Color = EntityColor;
		}
	}
}
