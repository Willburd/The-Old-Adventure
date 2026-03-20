using System.Linq;
using Godot;

public partial class SceneController : Node3D
{
	private Node3D _entity_container;

	public override void _Ready()
	{
		_entity_container = FindChild("Entities", true) as Node3D;
	}

	public override void _Process(double delta)
	{
		
	}

	public void ClearAllEntities()
	{
		foreach(Node3D node in _entity_container.GetChildren().Cast<Node3D>()) node.Free();
	}
	public void LoadEntityLayer()
	{
		
	}

	public void ReloadEntities()
	{
		ClearAllEntities();
		LoadEntityLayer();
	}
}
