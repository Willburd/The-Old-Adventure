using Godot;

/// <summary>
/// Container for globally accessible nodes like the world camera, render texture, and loaded scene container.
/// </summary>
public static class Game
{
	static public Node3D Root;

	static public Camera3D RenderCamera;
	static public Camera3D WorldCamera;
	static public Camera3D SkyCamera; 
	static public Camera3D PauseCamera;

	
	static public Viewport WorldViewport;
	static public Viewport SkyViewport;
	static public Viewport PauseViewport;

	static public TextureRect RenderTexture;
	static public Node3D LoadedScenesParent;

	/// <summary>
	/// Loads a scene and directly instantiates it. This is blocking and not async.
	/// </summary>
	public static Node3D LoadSceneFromPath(string path)
	{
		GD.Print("Direct load scene: " + path);

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

		GD.Print("Loaded instance: " + instance.Name);
		LoadedScenesParent.AddChild(instance);
		return instance;
	}

	/// <summary>
	/// Unloads are scenes loaded by the game.
	/// </summary>
	public static void UnloadAllScenes()
	{
		foreach(Node3D node in LoadedScenesParent.GetChildren())
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

public partial class __Boot : Node3D
{
	[Export]
	private string StartScenePath;

	public override void _Ready()
	{
		Game.Root = this;

		// Setup static container
		Game.RenderCamera = FindChild("RenderCamera", true) as Camera3D;
		Game.WorldCamera = FindChild("WorldCamera", true) as Camera3D;
		Game.SkyCamera = FindChild("SkyCamera", true) as Camera3D;
		Game.PauseCamera = FindChild("PauseCamera", true) as Camera3D;

		Game.WorldViewport = FindChild("WorldView", true) as Viewport;
		Game.SkyViewport = FindChild("SkyView", true) as Viewport;
		Game.PauseViewport = FindChild("PauseView", true) as Viewport;

		Game.RenderTexture = FindChild("RenderTexture", true) as TextureRect;
		Game.LoadedScenesParent = FindChild("LoadedScenes", true) as Node3D;

		// Load first scene
		Game.LoadSceneFromPath(StartScenePath);
	}
}
