using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RealEngine;

namespace Sandbox {
	class Game : Entity {
		public Vector2 GridSize = new Vector2(10, 10);
		public float Padding = 0.05f;
		public Entity ReferenceGrid;
		public Color DefaultGridColor;

		public Color SnakeColor;
		public Color SnakeBodyColor;
		public Color FruitColor;

		private Random m_Random = new Random();
		private Vector2 m_FruitPos;

		private Snake m_Snake;
		private SpriteRendererComponent[,] m_Grid;

		void ReGenerateFruitPos() {
			m_FruitPos = new Vector2(m_Random.Next((int)GridSize.X), m_Random.Next((int)GridSize.Y));

			if (m_Snake.SnakePosition == m_FruitPos)
				ReGenerateFruitPos();

			foreach (BodyPos pos in m_Snake.BodyPositions) {
				if (pos.Positon == m_FruitPos)
					ReGenerateFruitPos();
			}

		}

		void OnCreate() {
			m_Snake = new Snake();
			m_Snake.OnCreate();

			//Init Grid Tiles
			m_Grid = new SpriteRendererComponent[(int)GridSize.X, (int)GridSize.Y];

			Vector3 pos = Vector3.Zero;
			for (int x = 0; x < GridSize.X; x++) {
				for (int y = 0; y < GridSize.Y; y++) {
					pos.X = -(GridSize.X / 2) + (x + (x * Padding));
					pos.Y = -(GridSize.Y / 2) + (y + (y * Padding));
					m_Grid[x, y] = Instantiate(ReferenceGrid, ref pos).GetComponent<SpriteRendererComponent>();
				}
			}

			ReGenerateFruitPos();
		}

		void OnUpdate(float ts) {
			//Reset Grid Colors
			for (int x = 0; x < GridSize.X; x++) {
				for (int y = 0; y < GridSize.Y; y++) {
					m_Grid[x, y].Color = DefaultGridColor;
				}
			}

			//Update Snake
			m_Snake.OnUpdate(ts);

			//Check Collistions
			if(m_Snake.SnakePosition == m_FruitPos) {
				m_Snake.AddBodySegment();
				ReGenerateFruitPos();
			}

			if(m_Snake.SnakePosition.X >= GridSize.X
				|| m_Snake.SnakePosition.Y >= GridSize.Y
				|| m_Snake.SnakePosition.X < 0
				|| m_Snake.SnakePosition.Y < 0)
				m_Snake.Reset();

			foreach (BodyPos pos in m_Snake.BodyPositions)
				if (pos.Positon == m_Snake.SnakePosition) 
					m_Snake.Reset();

			//Draw Everything
			m_Grid[(int)m_FruitPos.X, (int)m_FruitPos.Y].Color = FruitColor;
			
			m_Grid[(int)m_Snake.SnakePosition.X, (int)m_Snake.SnakePosition.Y].Color = SnakeColor;
			
			foreach (BodyPos pos in m_Snake.BodyPositions)
				m_Grid[(int)pos.Positon.X, (int)pos.Positon.Y].Color = SnakeBodyColor;
		}
	}
}
