using Engine;
using EntComponents;
using System.Numerics;
using Assets;
using EntComponents.ActorBehavior;

namespace TestAdventure
{
    public class TestRoom(Room.EntranceType entrance_used) : Room("room_test", entrance_used)
    {
        public override void LoadAssets()
        {
            // Shaders
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);

            // Textures
            TextureData terrain_tex = AssetLoader.TextureAssetLoad("testroom_terrain", "sign_wood.png");

            // Materials
            AssetLoader.MaterialAssetLoad("testroom_terrain", new([terrain_tex], new() { { "uTexture0", 0 } }, standard_shader));

            // Models
            AssetLoader.ModelAssetLoad("testroom_terrain", "test_room.fbx");
            AssetLoader.ModelAssetLoad("testroom_terrain", "animation_test.fbx", AssetLoader.AssetSource.engine);

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
            for (int i = 0; i < 20; i++)
            {
                Actor fire = EntityFactory.CreateActor("fire_" + i, "actor_effectfire", new Transform( new Vector3( Tools.RandRange(-10, 10), Tools.RandRange(0, 10), Tools.RandRange(-10, 10)), Quaternion.Identity, Vector3.One), this, AssetLoader.AssetSource.engine);
                FireActorBehavior fire_behavior = (FireActorBehavior)fire.GetComponent(typeof(FireActorBehavior));
                new LevitatingBob(fire);
            }
        }

        public override void LoadExits()
        {
            CreateExitTrigger("exit_cave", 0, new Vector3(-2f, -1f, 36f), new(2f, 13f), new(typeof(TestRoom), EntranceType.caveA));
        }

        public override void LoadPlayer(EntranceType entrance_used)
        {
            switch (entrance_used)
            {
                default:
                case EntranceType.caveA:
                    CreatePlayer(new Vector3(-0.45f, -1.28f, 21.8f), Quaternion.Identity);
                    break;

                case EntranceType.debug:
                    CreatePlayer(new Vector3(0f, 0f, 5f), Quaternion.Identity);
                    break;
            }
        }

        public override void OnRoomUpdate()
        {

        }
    }
}