using Godot;
using System;

namespace EntComponents
{
	public partial class ExampleBehavior : Behavior
	{
		public override void OnEntityTick(double delta)
		{
			NodeParent.Position += new Vector3(0f,(float)-delta * 0.25f,0f);
		}
	}
}