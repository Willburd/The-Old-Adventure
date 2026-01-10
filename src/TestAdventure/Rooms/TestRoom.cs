using Engine;
using EntComponents;
using System.Numerics;
using Rendering;
using System.Drawing;

namespace TestAdventure
{
    public class TestRoom() : Room()
    {
        public override void LoadAssets()
        {
            // Shaders
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);
            
            // Textures
            TextureData skybox_day_tex = AssetLoader.TextureAssetLoad( "skybox_day", AssetLoader.AssetDirectoryAdventure + "/Textures/skybox_day.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData terrain_tex = AssetLoader.TextureAssetLoad( "testroom_terrain", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            
            // Materials
            MaterialData test_mat = AssetLoader.MaterialAssetLoad( "testroom_terrain", new( [terrain_tex], [new("uTexture0", 0)], standard_shader));
            MaterialData skybox_mat = AssetLoader.MaterialAssetLoad( "testroom_skybox", new( [skybox_day_tex], [new("uTexture0", 0)], standard_shader));
            
            // Model
            AssetLoader.ModelAssetLoad( "testroom_terrain", AssetLoader.AssetDirectoryAdventure + "/Models/test_room.fbx");

            // Environment
            Environment = new( new Vector4( 0.25f, 0.46f, 0.79f, 1f), 300f, new Vector4(1f,0.95f,0.95f,1f));
            Skybox skybox_model = new Skybox(this);
            skybox_model.SetModel( AssetLoader.ModelAssetGet("cube_map", AssetLoader.AssetSource.engine), skybox_mat);
        }

        public override void LoadActors()
        {
            // Create renderer model, move us into place.
            WorldRender? renderer = (WorldRender?)GetComponent(typeof(WorldRender));
            renderer?.SetModel( AssetLoader.ModelAssetGet("testroom_terrain"), AssetLoader.MaterialAssetGet("testroom_terrain"));

            // Set collider
            Collider? terrain_collider = (Collider?)GetComponent(typeof(Collider));
            terrain_collider?.SetShape( new Engine.ColliderShapes.WorldGeometryCol(renderer.GetMeshByName("col.001")));

            Light glow_test = new Light(this);
            glow_test.SetData( new Vector4(1f,1f,1f,1f), 12f, new Vector3(4f,2f,8f));

            /*
            // Move camera
            Camera.WorldCamera?.Position = new Vector3(0f,0.5f,1f);


            // Default actors

            EntityFactory.CreateActor( "actor_test", new Transform( new Vector3(0f,0f,0f), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.adventure);
            
            float radius = 6f;
            
            EntityFactory.CreateActor( "actor_pointer", new Transform( new Vector3(0f,0f,radius)), this, AssetLoader.AssetSource.engine);
            
            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(radius,0f,0f), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);

            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(-radius,0f,0f), Quaternion.CreateFromAxisAngle(Tools.Up, 45f) * Quaternion.CreateFromAxisAngle(Tools.Right, 45f)), this, AssetLoader.AssetSource.engine);

            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(0f,0f,-radius), Quaternion.CreateFromAxisAngle(Tools.Up, 45f) * Quaternion.CreateFromAxisAngle(Tools.Right, 45f), Vector3.One * 0.15f), this, AssetLoader.AssetSource.engine);
            */
        }

        public override void LoadExits()
        {
            exit_list.Add(typeof(TestRoom));
        }

        public override void OnRoomUpdate()
        {
            
        }
    }
}