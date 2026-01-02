using Engine;
using EntComponents;
using System.Numerics;
using Rendering;

namespace TestAdventure
{
    public class TestRoom() : Room()
    {
        public override void LoadAssets()
        {
            // Shaders
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);
            
            // Textures
            TextureData terrain_tex = AssetLoader.TextureAssetLoad( "testroom_terrain", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png");
            
            // Materials
            AssetLoader.MaterialAssetLoad( "testroom_terrain", new( [terrain_tex], [new MaterialUniformData("uTexture0", 0)], standard_shader));
            
            // Model
            AssetLoader.ModelAssetLoad( "testroom_terrain", AssetLoader.AssetDirectoryEngine + "/Models/cube.fbx");
        }

        public override void LoadActors()
        {
            // Create renderer model, move us into place.
            WorldRender? renderer = (WorldRender?)GetComponent(typeof(WorldRender));
            renderer?.SetModel( AssetLoader.ModelAssetGet("testroom_terrain"), AssetLoader.MaterialAssetGet("testroom_terrain"));

            Position += new Vector3(0f,-1.5f,0f);
            Scale *= new Vector3(125f,1f,125f);


            // Default actors
            new EditorCamera(new Transform( new Vector3(0f,0f,0f)), true, this);

            EntityFactory.CreateActor( "actor_pointer", new Transform( new Vector3(0f,0f,0f)), this, AssetLoader.AssetSource.engine);
            
            float radius = 6f;
            
            EntityFactory.CreateActor( "actor_test", new Transform( new Vector3(0f,-1f,radius), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.adventure);
            
            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(radius,-1f,0f), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);

            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(-radius,-1f,0f), Quaternion.CreateFromAxisAngle(Tools.Up, 45f) * Quaternion.CreateFromAxisAngle(Tools.Right, 45f)), this, AssetLoader.AssetSource.engine);

            EntityFactory.CreateActor( "actor_cube", new Transform( new Vector3(0f,-1f,-radius), Quaternion.CreateFromAxisAngle(Tools.Up, 45f) * Quaternion.CreateFromAxisAngle(Tools.Right, 45f), Vector3.One * 0.15f), this, AssetLoader.AssetSource.engine);
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