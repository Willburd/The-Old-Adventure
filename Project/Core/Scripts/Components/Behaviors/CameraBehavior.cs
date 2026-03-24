using Godot;
using StateMachines;

namespace EntComponents
{
	public partial class CameraBehavior : Behavior
	{

        public override void _Ready()
		{
			CurrentState = new CameraState_FollowPlayer(this);
		}

		public override void _Input(InputEvent @event)
		{
			CurrentState.InputHandler(@event);
		}
	}
}