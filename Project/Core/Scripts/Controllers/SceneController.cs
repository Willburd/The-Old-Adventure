using System.Linq;
using Godot;

public partial class SceneController : Node3D
{
	[Export]
	private Node _entity_container;
	[Export]
	private Node _layer_container;

	private string _current_set_layer;

	public Node LayerContainer { get { return _layer_container; } }
	public Node EntityContainer { get { return _entity_container; } }

	public override void _Ready()
	{
		_layer_container = FindChild("Layers", false);
		_entity_container = FindChild("Entities", false);
		SceneStarted();
	}

	/// <summary>
	/// Called when a scene finishes loading, override to handle layer setup and player spawning.
	/// </summary>
	protected virtual void SceneStarted()
	{
		// Example, don't call parent.
		LoadEntityLayer("Default");
		ClearLayerTemplates();
	}

	/// <summary>
	/// Clears all entities in a scene, does not respawn them.
	/// </summary>
	public void ClearAllEntities()
	{
		foreach(Node3D node in _entity_container.GetChildren().Cast<Node3D>()) node.Free();
	}

	/// <summary>
	/// Respawns all entities, does not clear them.
	/// </summary>
	public void RespawnAllEntities()
	{
		foreach(Node3D node in _entity_container.GetChildren().Cast<Node3D>())
		{
			if(node is Entity) (node as Entity).Respawn();
		}
	}

	/// <summary>
	/// Loads a specific layer of instances based on the game's current progression, such as story or time of day.
	/// </summary>
	public void LoadEntityLayer(string layer_name)
	{
		GD.Print("==Started loading layer: " + Name + " :> " + layer_name);
		_current_set_layer = layer_name;
		Node layer = _layer_container.FindChild(_current_set_layer, false);
		if(layer == null) 
		{
			GD.Print("==Failed to load layer: " + layer_name);
			return;
		}
		foreach(Node3D ent in layer.GetChildren().Cast<Node3D>())
		{
			if(ent is not Entity) continue;
			(ent as Entity).Spawn(this);
		}
		GD.Print("==Finished loading layer: " + Name + " :> " + layer_name);
	}

	/// <summary>
	/// Called after all layers are loaded to delete all template entities.
	/// </summary>
	public void ClearLayerTemplates()
	{
		foreach(Node layer in _layer_container.GetChildren().Cast<Node>())
		{
			layer.Free();
		}
	}

	/// <summary>
	/// Refreshes a scene as if you entered it from another scene. Clearing all entities and respawning them.
	/// </summary>
	public void ReloadEntities()
	{
		ClearAllEntities();
		LoadEntityLayer(_current_set_layer);
	}
}
