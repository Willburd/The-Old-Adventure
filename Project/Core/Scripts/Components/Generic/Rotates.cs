using Godot;

namespace EntComponents
{
	public partial class Rotates : Component
	{
		[Export]
		private Vector3 _rotation_rate;
		public override void OnEntityTick(double delta)
		{
			NodeParent.Rotation += _rotation_rate * (float)delta;
		}
	}
}