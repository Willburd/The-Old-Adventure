using Godot;
using System;

public partial class SkyRotator : WorldEnvironment
{
	[Export]
	private Vector3 _rotation_rate;
	public override void _Process(double delta)
	{
		Environment.SkyRotation += _rotation_rate * (float)delta;
	}
}
