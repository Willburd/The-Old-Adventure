using System.Linq;
using Godot;

namespace GameScenes
{
	public partial class SceneController : Node3D
	{
		private Node _entity_container;
		private Node _layer_container;
		private Node _entrance_container;

		private string _current_set_layer;

		public Node LayerContainer { get { return _layer_container; } }
		public Node EntityContainer { get { return _entity_container; } }
		public Node EntranceContainer { get { return _entrance_container; } }

		public override void _Ready()
		{
			// Made all template layers disable processing
			_layer_container = FindChild("Layers", false);
			foreach(Node layer in _layer_container.GetChildren())
			{
				foreach(Node3D node in layer.GetChildren(true).Cast<Node3D>())
				{
					node.ProcessMode = ProcessModeEnum.Disabled;
					node.Hide();
				}
			}
			// Prepare entity container
            _entity_container = new Node
            {
                Name = "Entities"
            };
            AddChild(_entity_container);
			// Entrances now too
			_entrance_container = FindChild("Entrances", false);
			// Enter scene
			EnterScene(Game.CurrentEntrance);
			SceneStarted(Game.CurrentEntrance);
		}

		/// <summary>
		/// Called when a scene finishes loading, override to handle layer setup and player spawning.
		/// </summary>
		protected virtual void SceneStarted(string entrance_id) {}

		/// <summary>
		/// Clears all entities in a scene, does not respawn them.
		/// </summary>
		public void ClearAllEntities()
		{
			foreach(Node3D node in _entity_container.GetChildren().Cast<Node3D>()) node.Free();
		}

		/// <summary>
		/// Refreshes a scene as if you entered it from another scene. Clearing all entities and respawning them.
		/// </summary>
		public void ReloadEntities()
		{
			ClearAllEntities();
			LoadEntityLayer(_current_set_layer);
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
			foreach(Node3D node in layer.GetChildren().Cast<Node3D>())
			{
				Game.LoadEntityFromPath(node.SceneFilePath, this, node.GlobalPosition, node.GlobalRotation);
			}
			GD.Print("==Finished loading layer: " + Name + " :> " + layer_name);
		}

		public void EnterScene(string entrance_id)
		{
			Entrance ent = (Entrance)_entrance_container.FindChild(entrance_id, false);
			ent ??= (Entrance)_entrance_container.FindChild("Default", false); // Backup
			GD.Print("Loading entrance " + ((ent == null) ? "Err" : ent.Name));

			// Spawn player
			Vector3 spawn_pos = (ent == null) ? Vector3.Zero : ent.SpawnPosition();
			Vector3 spawn_rot = (ent == null) ? Vector3.Zero : ent.SpawnRotation();
			Game.LoadEntityFromPath(Game.PlayerResourcePath, this, spawn_pos, spawn_rot);

			// Spawn camera
			Vector3 camera_pos = (ent == null) ? Vector3.Back : ent.CameraPosition();
			Vector3 camera_rot = (ent == null) ? Vector3.Zero : ent.CameraRotation();
			Game.LoadEntityFromPath(Game.CameraResourcePath, this, camera_pos, camera_rot);
		}
	}
}