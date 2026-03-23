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
	
	static public ViewFade ViewFade;

	static public TextureRect RenderTexture;
	static public Node LoadedScenesParent;
	static public CharacterBody3D CurrentPlayer;

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
	public static Node3D LoadEntityFromPath(string path, GameScenes.SceneController host_scene, Vector3 at_pos, Vector3 at_rotation)
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

		// Setup the entity as an actually loaded entity and not just a template in a layer
		host_scene.EntityContainer.AddChild(instance);
		instance.GlobalPosition = at_pos;
		instance.GlobalRotation = at_rotation;

		GD.Print(" -Loaded entity: " + instance.Name);
		return instance;
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

	public static void SetFadeColor(Color goal_fade)
	{
		ViewFade.GoalFade = goal_fade;
	}

	
	public static Vector3 GetGravity(double delta)
	{
		return (Vector3)ProjectSettings.GetSetting("physics/3d/default_gravity_vector") * ((float)ProjectSettings.GetSetting("physics/3d/default_gravity") * (float)delta);
	}

	public static void Shutdown()
	{
		Root.GetTree().Quit();
	}
}
