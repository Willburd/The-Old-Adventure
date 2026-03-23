using Godot;
using EntComponents;

namespace StateMachines
{
    public class CameraState(Behavior owner) : StateMachine(owner)
    {

        public void TargetMove(Vector3 goal, double delta, float rate = float.PositiveInfinity)
        {
            Behavior.NodeParent.GlobalPosition = Tools.StepToward(Behavior.NodeParent.GlobalPosition, goal, rate * (float)delta);
        }

        public Vector3 GetTarget()
        {
            return Behavior.NodeParent.GlobalPosition;
        }

        public void CameraMove(Vector3 goal, double delta, float rate = float.PositiveInfinity)
        {
            Game.WorldCamera.GlobalPosition = Tools.StepToward(Game.WorldCamera.GlobalPosition, goal, rate * (float)delta);
        }

        public void CameraLookAt(Vector3 target, Vector3 up)
        {
            if(target == Game.WorldCamera.GlobalPosition) return;
            Game.WorldCamera.LookAt(target, up);
        }
    }
}