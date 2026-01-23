using System.Numerics;
using Engine;
using Assets;

namespace EntComponents.ActorBehavior
{
    /// <summary>
    /// Entity Component that renders the standard gameplay hud.
    /// </summary>
    public class HudActorBehavior(Entity host_entity) : WorldRender(host_entity)
    {

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.load_assets, Core.Signals.create, Core.Signals.render_priority, Core.Signals.update, Core.Signals.hud_render];
        }

        protected override uint HandleAssetLoad()
        {
            // Shaders
            ShaderData shader_sprite = AssetLoader.ShaderAssetGet("sprite2d", AssetLoader.AssetSource.engine);

            // Textures
            TextureData button_texture = AssetLoader.TextureAssetLoad("Hud/HudButton.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData heart_empty_texture = AssetLoader.TextureAssetLoad("Hud/HealthBack.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData heart_quarter_texture = AssetLoader.TextureAssetLoad("Hud/HealthQuarter.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData heart_half_texture = AssetLoader.TextureAssetLoad("Hud/HealthHalf.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData heart_three_quarters_texture = AssetLoader.TextureAssetLoad("Hud/HealthThreeQuarter.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);
            TextureData heart_full_texture = AssetLoader.TextureAssetLoad("Hud/HealthFull.png", Silk.NET.OpenGL.TextureTarget.Texture2D, AssetLoader.AssetSource.engine);

            // Materials
            AssetLoader.MaterialAssetLoad("hud_button", new([button_texture], [new("uTexture0", 0)], shader_sprite), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad("hud_heart_backing", new([heart_empty_texture], [new("uTexture0", 0)], shader_sprite), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad("hud_heart_quarter", new([heart_quarter_texture], [new("uTexture0", 0)], shader_sprite), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad("hud_heart_half", new([heart_half_texture], [new("uTexture0", 0)], shader_sprite), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad("hud_heart_three_quarters", new([heart_three_quarters_texture], [new("uTexture0", 0)], shader_sprite), AssetLoader.AssetSource.engine);
            AssetLoader.MaterialAssetLoad("hud_heart_full", new([heart_full_texture], [new("uTexture0", 0)], shader_sprite), AssetLoader.AssetSource.engine);

            return 1;
        }

        protected override uint HandleCreate()
        {

            return 1;
        }

        protected override uint HandleUpdate()
        {

            return 1;
        }

        protected override uint HandleHudRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            Vector3 unit = HudUnit();
            Vector3 topright = HudTopRight();
            Vector3 topleft = HudTopLeft();

            Core.RenderSprite(AssetLoader.MaterialAssetGet("hud_button", AssetLoader.AssetSource.engine), topright + new Vector3(-0.05f, -0.11f, 0f), Vector3.One * 0.086f, new Vector3(1f, 1f, 0f), vertex_uniforms);
            Core.RenderSprite(AssetLoader.MaterialAssetGet("hud_button", AssetLoader.AssetSource.engine), topright + new Vector3(-0.13f, -0.06f, 0f), Vector3.One * 0.086f, new Vector3(1f, 1f, 0f), vertex_uniforms);
            Core.RenderSprite(AssetLoader.MaterialAssetGet("hud_button", AssetLoader.AssetSource.engine), topright + new Vector3(-0.21f, -0.11f, 0f), Vector3.One * 0.086f, new Vector3(1f, 1f, 0f), vertex_uniforms);
            
            Core.RenderSprite(AssetLoader.MaterialAssetGet("hud_button", AssetLoader.AssetSource.engine), topright + new Vector3(-0.13f, -0.16f, 0f), Vector3.One * 0.1f, new Vector3(0.2f, 0.6f, 1f), vertex_uniforms);
            
            //Core.RenderText2D($"This is a test\nof the hud\nrender system.", HudLerp(0.5f + MathF.Cos(0) * 0.5f, 0.5f + MathF.Sin(0) * 0.5f), Vector3.One * 0.1f, vertex_uniforms);

            Core.RenderSprite(AssetLoader.MaterialAssetGet("hud_heart_backing", AssetLoader.AssetSource.engine), topleft + new Vector3(0.04f, -0.04f, 0f), Vector3.One * 0.05f, new Vector3(1f, 1f, 1f), vertex_uniforms);
            Core.RenderSprite(AssetLoader.MaterialAssetGet("hud_heart_full", AssetLoader.AssetSource.engine), topleft + new Vector3(0.04f, -0.04f, 0f), Vector3.One * 0.06f, new Vector3(1f, 1f, 1f), vertex_uniforms);
            
            return 1;
        }
    }
}