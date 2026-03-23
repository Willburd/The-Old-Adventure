using Godot;

public partial class ViewFade : TextureRect
{
	[Export]
	private float _fade_rate = 1f;
	[Export]
	public Color GoalFade = new(1f,1f,1f,0f);
	public override void _Process(double delta)
	{
		Modulate = Tools.StepToward(Modulate, GoalFade, _fade_rate * (float)delta);
	}
}
