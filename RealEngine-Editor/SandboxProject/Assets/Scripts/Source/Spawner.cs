using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using RealEngine;

namespace Sandbox {
	public class Spawner : Entity {
		public Vector3 translation;
		public Entity copy;

		void OnCreate() {
			if(copy)
				Instantiate(copy, ref translation);
		}
	}
}
