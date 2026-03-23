using StateMachines;
using Godot;

namespace EntComponents
{
	public partial class PlayerBehavior : Behavior
	{
		public CharacterBody3D PlayerParent
		{
			get
			{
				return (CharacterBody3D)GetParent();
			}
		}

        public override void _Ready()
		{
			CurrentState = new PlayerState_Ground(this);
		}

		public bool OnGround()
		{
			return PlayerParent.IsOnFloor();
		}
	}
}