using Godot;
using System;

public partial class SkyCamera : Camera3D
{
	public override void _Ready()
	{
		RenderingServer.FramePreDraw += () => SnapToWorldCamera();
		GlobalPosition = Vector3.Zero;
	}

	/// <summary>
	/// Snap to the position and angle of the assigned world camera.
	/// </summary>
	private void SnapToWorldCamera()
	{
		Rotation = Game.WorldCamera.Rotation;
	}
}
