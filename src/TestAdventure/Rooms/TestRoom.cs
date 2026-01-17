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
            TextureData terrain_tex = AssetLoader.TextureAssetLoad("testroom_terrain", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png", Silk.NET.OpenGL.TextureTarget.Texture2D);

            // Materials
            AssetLoader.MaterialAssetLoad("testroom_terrain", new([terrain_tex], [new("uTexture0", 0)], standard_shader));

            // Models
            AssetLoader.ModelAssetLoad("testroom_terrain", AssetLoader.AssetDirectoryAdventure + "/Models/test_room.fbx");

            // Environments
            Environment = AssetLoader.EnvironmentAssetGet("standard_daynight", AssetLoader.AssetSource.engine);
        }

        public override void LoadActors()
        {
            // Create renderer model, move us into place.
            WorldRender renderer = (WorldRender)GetComponent(typeof(WorldRender));
            renderer.SetModel(AssetLoader.ModelAssetGet("testroom_terrain"), AssetLoader.MaterialAssetGet("testroom_terrain"));

            // Set collider
            Collider terrain_collider = (Collider)GetComponent(typeof(Collider));
            terrain_collider.SetShape(new Engine.ColliderShapes.WorldGeometryCol(renderer.GetMeshByName("col.001")));
            terrain_collider.CollisionMask = Collider.mask_worldgeo;

            // Default actors
            EntityFactory.CreateActor(PlayerActorBehavior.player_actor_id, "actor_player", new Transform(new Vector3(0f, 0f, 5f), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);

            for (int i = 0; i < 20; i++)
            {
                Actor fire = EntityFactory.CreateActor("fire_" + i, "actor_effectfire", new Transform( new Vector3( Tools.RandRange(-10, 10), Tools.RandRange(0, 10), Tools.RandRange(-10, 10)), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);
                FireActorBehavior fire_behavior = (FireActorBehavior)fire.GetComponent(typeof(FireActorBehavior));
                fire_behavior.DecayTimer = (int)Tools.RandRange(400, 590);
                // new LevitatingBob(fire);
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