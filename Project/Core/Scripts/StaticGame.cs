using System.Linq;
using Godot;

/// <summary>
/// Container for globally accessible nodes like the world camera, render texture, and loaded scene container.
/// </summary>
public static class Game
{
	public enum States
	{
		active,
		cutscene,
		pause,
		menu,
		title,
	}

	static public States CurrentState = States.active;
	static public Node3D Root;

	static public Camera3D RenderCamera;
	static public Camera3D WorldCamera;
	static public Camera3D SkyCamera; 
	static public Camera3D PauseCamera;

	
	static public Viewport WorldViewport;
	static public Viewport SkyViewport;
	static public Viewport PauseViewport;

	static public TextureRect RenderTexture;
	static public Node LoadedScenesParent;

	/// <summary>
	/// Loads a scene and directly instantiates it. This is blocking and not async.
	/// </summary>
	public static Node3D LoadSceneFromPath(string path)
	{
		GD.Print("====================== Direct load scene: " + path + " ======================");

		PackedScene scene = GD.Load<PackedScene>(path);
		if(scene == null)
		{
			GD.Print(" -Failed to load scene from path.");
			return null;
		}

		Node3D instance = scene.Instantiate<Node3D>();
		if(instance == null)
		{
			GD.Print(" -Failed to instantiate scene.");
			return null;
		}

		LoadedScenesParent.AddChild(instance);

		GD.Print("====================== Finished Loading scene: " + instance.Name + " ======================");

		return instance;
	}

	/// <summary>
	/// Spawns an entity at a set position using an asset path.
	/// </summary>
	public static Entity LoadEntityFromPath(string path, GameScenes.SceneController host_scene, Vector3 at_pos, Vector3 at_rotation)
	{
		GD.Print("Direct load entity: " + path);

		PackedScene scene = GD.Load<PackedScene>(path);
		if(scene == null)
		{
			GD.Print(" -Failed to load entity from path.");
			return null;
		}

		Node3D instance = scene.Instantiate<Node3D>();
		if(instance == null)
		{
			GD.Print(" -Failed to instantiate entity.");
			return null;
		}
        if (instance is not Entity)
        {
            GD.Print(" -Instantiated node was not an entity.");
			instance.Free();
            return null;
        }

		// Setup the entity as an actually loaded entity and not just a template in a layer
        Entity ent = instance as Entity;
		host_scene.EntityContainer.AddChild(ent);
		ent.Setup(host_scene, at_pos, at_rotation);

		GD.Print(" -Loaded entity: " + ent.Name);
		return ent;
	}

	/// <summary>
	/// Unloads are scenes loaded by the game.
	/// </summary>
	public static void UnloadAllScenes()
	{
		foreach(Node3D node in LoadedScenesParent.GetChildren().Cast<Node3D>())
		{
			GD.Print("Unloaded instance: " + node.Name);
			node.Free();
		}
	}

	public static void Shutdown()
	{
		Root.GetTree().Quit();
	}
}
