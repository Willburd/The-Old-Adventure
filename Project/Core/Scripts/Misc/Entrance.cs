using Godot;
using System;

public partial class Entrance : Node3D
{
	private RayCast3D _spawn_at;
	private RayCast3D _run_goal;
	private Camera3D _entrance_camera;

	public override void _Ready()
	{
		_spawn_at = (RayCast3D)FindChild("Spawn");
		_run_goal = (RayCast3D)FindChild("RunGoal");
		_entrance_camera = (Camera3D)FindChild("PreviewCamera");
	}

	public Vector3 SpawnPosition()
	{
		if(_spawn_at.IsColliding())
		{
			return _spawn_at.GetCollisionPoint();
		}
		return GlobalPosition;
	}

	public Vector3 SpawnRotation()
	{
		Vector3 start = SpawnPosition();
		Vector3 end = RunGoalPosition();

		if(start.DistanceTo(end) < 0.2f)
		{
			return new Vector3(0f, GlobalRotation.Y, 0f);
		}
		return Tools.GetFlatDirEulars(start, end);
	}

	public Vector3 RunGoalPosition()
	{
		if(_run_goal.IsColliding())
		{
			return _run_goal.GetCollisionPoint();
		}
		return GlobalPosition;
	}

	public Vector3 CameraPosition()
	{
		return _entrance_camera.GlobalPosition;
	}
	
	public Vector3 CameraRotation()
	{
		return _entrance_camera.GlobalRotation;
	}
}
