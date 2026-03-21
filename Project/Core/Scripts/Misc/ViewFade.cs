using Godot;

public partial class ViewFade : TextureRect
{
	[Export]
	private float _fade_rate = 1f;
	[Export]
	private Color _goal_fade = new(1f,1f,1f,0f);
	public override void _Process(double delta)
	{
		_goal_fade = _goal_fade.Lerp(_goal_fade,_fade_rate * (float)delta);
	}
}
