using Godot;

public partial class Entity : Node3D
{
	public Game.States MaximumActiveState = Game.States.active;

	public bool CanTick()
	{
		return Game.CurrentState <= MaximumActiveState;
	}
}
