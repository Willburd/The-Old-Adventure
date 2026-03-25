using Godot;
using StateMachines;

namespace EntComponents
{
	public partial class CameraBehavior : Behavior
	{
		public RayCast3D CameraRay;

        public override void _Ready()
		{
			CurrentState = new CameraState_FollowPlayer(this);
			CameraRay = (RayCast3D)NodeParent.FindChild("CameraRay");
		}

		public override void _Input(InputEvent @event)
		{
			CurrentState.InputHandler(@event);
		}
	}
}