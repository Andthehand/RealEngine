using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using RealEngine;

namespace Sandbox {
	enum Direction {
		South = 0,
		North,
		East,
		West
	}
	struct BodyPos {
		public BodyPos(Vector2Int positon, Direction facing) {
			Positon = positon;
			Facing = facing;
		}

		public Vector2Int Positon;
		public Direction Facing;
	}

	class Snake {
		private float MoveTimeInterval = 0.25f;
		private float MoveDeltatime;

		public Vector2Int SnakePosition = Vector2Int.Zero;

		public List<BodyPos> BodyPositions;

		private Direction m_SnakeFacing;

		public void Reset() {
			MoveTimeInterval = 0.25f;
			SnakePosition = Vector2Int.Zero;
			m_SnakeFacing = Direction.South;
			BodyPositions.Clear();
		}

		public void OnCreate() {
			//Init Variables
			MoveDeltatime = MoveTimeInterval;
			BodyPositions = new List<BodyPos>();
		}

		public void OnUpdate(float ts) {
			//Inputs
			if (Input.IsKeyDown(KeyCode.W))
				m_SnakeFacing = Direction.North;
			else if (Input.IsKeyDown(KeyCode.S))
				m_SnakeFacing = Direction.South;

			if (Input.IsKeyDown(KeyCode.A))
				m_SnakeFacing = Direction.West;
			else if (Input.IsKeyDown(KeyCode.D))
				m_SnakeFacing = Direction.East;

			//Move Snake
			MoveDeltatime -= ts;
			if (MoveDeltatime <= 0) {
				MoveDeltatime = MoveTimeInterval;

				for (int i = BodyPositions.Count - 1; i >= 0; i--) {
					if (i == 0) {
						BodyPos pos = new BodyPos(SnakePosition, m_SnakeFacing);

						BodyPositions[i] = pos;
					}
					else {
						BodyPositions[i] = BodyPositions[i - 1];
					}
				}

				switch (m_SnakeFacing) {
					case Direction.North:
						SnakePosition.Y -= 1;
						break;
					case Direction.South:
						SnakePosition.Y += 1;
						break;
					case Direction.East:
						SnakePosition.X += 1;
						break;
					case Direction.West:
						SnakePosition.X -= 1;
						break;
				}
			}
		}

		public void AddBodySegment() {
			MoveTimeInterval -= .01f;

			if (BodyPositions.Count == 0) {
				Vector2Int newBodyPos = SnakePosition;
				switch (m_SnakeFacing) {
					case Direction.North:
						newBodyPos.Y += 1;
						break;
					case Direction.South:
						newBodyPos.Y -= 1;
						break;
					case Direction.East:
						newBodyPos.X -= 1;
						break;
					case Direction.West:
						newBodyPos.X += 1;
						break;
				}

				BodyPositions.Add(new BodyPos(newBodyPos, m_SnakeFacing));
			}
			else {
				Vector2Int newBodyPos = BodyPositions[BodyPositions.Count - 1].Positon;
				switch (BodyPositions[BodyPositions.Count - 1].Facing) {
					case Direction.North:
						newBodyPos.Y += 1;
						break;
					case Direction.South:
						newBodyPos.Y -= 1;
						break;
					case Direction.East:
						newBodyPos.X -= 1;
						break;
					case Direction.West:
						newBodyPos.X += 1;
						break;
				}

				BodyPos pos = new BodyPos(newBodyPos, BodyPositions[BodyPositions.Count - 1].Facing);
				BodyPositions.Add(pos);
			}
		}
	}
}
