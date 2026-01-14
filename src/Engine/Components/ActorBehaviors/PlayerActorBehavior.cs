using Engine;
using Silk.NET.Input;
using Assets;

namespace EntComponents.ActorBehavior
{
    public class PlayerActorBehavior : EntComponent
    {
        public const string player_actor_id = "global_id_actor_player";

        public PlayerActorBehavior(Entity host_entity) : base(host_entity)
        {
            current_state = new PlayerStates.Grounded(this);
            host_entity.MinimumRenderDistance = float.PositiveInfinity;
        }

        protected PlayerStates.PlayerState? current_state;

        public void SetPlayerState(PlayerStates.PlayerState? new_state)
        {
            current_state?.End(new_state);
            new_state?.Start(current_state);
            current_state = new_state;
            Console.WriteLine(current_state.GetType());
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.load_assets, Core.Signals.create, Core.Signals.update, Core.Signals.input_pressed, Core.Signals.input_released, Core.Signals.collision];
        }

        protected override uint HandleAssetLoad()
        {
            // Shaders
            ShaderData standard_shader = AssetLoader.ShaderAssetGet("standard", AssetLoader.AssetSource.engine);

            // Textures
            TextureData sign_wood = AssetLoader.TextureAssetLoad("sign_wood", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_wood.png", Silk.NET.OpenGL.TextureTarget.Texture2D);
            TextureData sign_face = AssetLoader.TextureAssetLoad("sign_face", AssetLoader.AssetDirectoryAdventure + "/Textures/sign_face.png", Silk.NET.OpenGL.TextureTarget.Texture2D);

            // Materials
            AssetLoader.MaterialAssetLoad("sign_wood", new([sign_wood], [new("uTexture0", 0)], standard_shader));
            AssetLoader.MaterialAssetLoad("sign_face", new([sign_face], [new("uTexture0", 0)], standard_shader));

            // Model
            AssetLoader.ModelAssetLoad("sign", AssetLoader.AssetDirectoryAdventure + "/Models/sign.fbx");

            return 1;
        }

        protected override uint HandleCreate()
        {
            // Set the render's model and materials
            WorldRender renderer = (WorldRender)Host.GetComponent(typeof(WorldRender));

            var sign_model = AssetLoader.ModelAssetGet("sign");
            renderer.SetModel(sign_model, AssetLoader.MaterialAssetGet("sign_wood"));
            renderer.SetMaterial(AssetLoader.MaterialAssetGet("sign_face"), sign_model.GetMeshIndex("Sign_1"));

            // Collision
            Collider collision = (Collider)Host.GetComponent(typeof(Collider));
            collision.SetShape(new Engine.ColliderShapes.CylinderCol(0.75f, 0.25f));
            collision.CollisionMask = Collider.mask_player;

            // Player handling
            SetPlayerState(new PlayerStates.Grounded(this));

            return 1;
        }

        protected override uint HandleUpdate()
        {
            current_state?.Process();
            return 1;
        }

        protected override uint HandlePressed(Key? key, ButtonName? button)
        {
            if (key == InputHandler.KeyIDCancel)
            {

                return 1;
            }
            return 0;
        }
        protected override uint HandleReleased(Key? key, ButtonName? button)
        {
            return 1;
        }

        protected override uint HandleCollisions(List<Collider.Collision> collisions)
        {
            return 1;
        }
    }
}