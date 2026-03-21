using Godot;

public partial class __Boot : Node3D
{
	[Export]
	private string _start_scene_path;

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
		Game.ViewFade = Game.RenderTexture.FindChild("ViewFade", false) as TextureRect;

		// Create scene container
        Game.LoadedScenesParent = new Node
        {
            Name = "LoadedScenes"
        };
		Game.Root.AddChild(Game.LoadedScenesParent);

        // Load first scene
        Game.LoadSceneFromPath(_start_scene_path);
		Game.SetFadeColor(new Color(1f,1f,1f,0f));
	}
}
