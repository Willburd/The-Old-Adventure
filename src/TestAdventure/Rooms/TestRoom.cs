using Engine;
using EntComponents;
using System.Numerics;
using Assets;
using EntComponents.ActorBehavior;

namespace TestAdventure
{
    public class TestRoom(string room_id) : Room(room_id)
    {
        public override void LoadAssets()
        {
            // Shaders
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);
            
            // Textures
            TextureData terrain_tex = AssetLoader.TextureAssetLoad( "testroom_terrain", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            
            // Materials
            AssetLoader.MaterialAssetLoad( "testroom_terrain", new( [terrain_tex], [new("uTexture0", 0)], standard_shader));
            
            // Models
            AssetLoader.ModelAssetLoad( "testroom_terrain", AssetLoader.AssetDirectoryAdventure + "/Models/test_room.fbx");

            // Environments
            Environment = AssetLoader.EnvironmentAssetGet("standard_daynight", AssetLoader.AssetSource.engine);
        }

        public override void LoadActors()
        {
            // Create renderer model, move us into place.
            WorldRender renderer = (WorldRender)GetComponent(typeof(WorldRender));
            renderer.SetModel( AssetLoader.ModelAssetGet("testroom_terrain"), AssetLoader.MaterialAssetGet("testroom_terrain"));

            // Set collider
            Collider terrain_collider = (Collider)GetComponent(typeof(Collider));
            terrain_collider.SetShape( new Engine.ColliderShapes.WorldGeometryCol(renderer.GetMeshByName("col.001")));
            terrain_collider.CollisionMask = Collider.mask_worldgeo;

            Light glow_test = new Light(this);
            glow_test.SetData( new Vector4(1f,1f,1f,1f), 12f, new Vector3(4f,2f,8f));

            // Move camera
            Camera.WorldCamera.Position = new Vector3(0f,0.5f,1f);

            // Default actors
            EntityFactory.CreateActor( PlayerActorBehavior.player_actor_id, "actor_player", new Transform( new Vector3(0f,0f,5f), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);


            // WTF IS GOING ON HERE
            AssetLoader.ModelAssetLoad("test_pointer", AssetLoader.AssetDirectoryEngine + "/Models/pointer.fbx", AssetLoader.AssetSource.engine);
            for(int i = 0; i < 360; i += 45)
            {
                float angle = i;
                float offset = i / 25f;

                Actor temp = new(new Transform(new Vector3(offset, 1f, 0f)), "pointA"+angle, null, this);
                WorldRender render = new(temp);
                Rotates rot = new Rotates(temp);
                rot.rotation = new Vector3(0f, 0.01f, 0f);
                render.SetModel(AssetLoader.ModelAssetGet("test_pointer",AssetLoader.AssetSource.engine), AssetLoader.MaterialAssetGet("debug_normals", AssetLoader.AssetSource.engine));
                temp.Rotation = Quaternion.CreateFromAxisAngle(Tools.Up, Tools.DegreesToRadians(angle));

                Actor new_temp = new(new Transform(new Vector3(offset, 2f, 0f)), "pointB"+angle, null, this);
                rot = new Rotates(new_temp);
                rot.sync_to_actor = temp;
                WorldRender new_render = new(new_temp);
                new_render.SetModel(AssetLoader.ModelAssetGet("test_pointer",AssetLoader.AssetSource.engine), AssetLoader.MaterialAssetGet("debug_normals", AssetLoader.AssetSource.engine));
            }
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