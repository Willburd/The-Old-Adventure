using Godot;

public partial class Entity : Node3D
{
	private SceneController _host_scene;
	private bool _is_prefab = true;
	private Vector3 _starting_position;
	private Vector3 _starting_rotation;

	public void Setup(SceneController host, Vector3 new_pos, Vector3 at_rotation)
	{
		_host_scene = host;
		GlobalPosition = new_pos;
		GlobalRotation = at_rotation;

		_starting_position = GlobalPosition;
		_starting_rotation = GlobalRotation;
		_is_prefab = false;
	}

	public void Spawn(SceneController scene)
	{
		Game.LoadEntityFromPath(SceneFilePath, scene, GlobalPosition, GlobalRotation);
	}

	public void Respawn()
	{
		Game.LoadEntityFromPath(SceneFilePath, _host_scene, _starting_position, _starting_rotation);
		Free();
	}

	public Game.States MaximumActiveState = Game.States.active;

	public bool CanTick()
	{
		return Game.CurrentState <= MaximumActiveState;
	}
}
