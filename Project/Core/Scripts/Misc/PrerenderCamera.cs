using Godot;

public partial class PrerenderCamera : Camera3D
{
	public override void _Ready()
	{
		RenderingServer.FramePreDraw += () => SnapToWorldCamera();
	}

	/// <summary>
	/// Snap to the position and angle of the assigned world camera.
	/// </summary>
	private void SnapToWorldCamera()
	{
		Environment = Game.WorldCamera.Environment;
		GlobalPosition = Game.WorldCamera.GlobalPosition;
		GlobalRotation = Game.WorldCamera.GlobalRotation;
	}
}
