using System.Numerics;
using Assets;
using Engine;

namespace EntComponents.ActorBehavior
{
    public class FadeInActorBehavior(Entity host_entity) : FadeoutActorBehavior(host_entity)
    {
        protected override uint HandleHudRender(double tick_delta, Dictionary<string, object> vertex_uniforms)
        {
            MaterialData fade_mat = AssetLoader.MaterialAssetGet("fade", AssetLoader.AssetSource.engine);
            fade_mat.Uniforms["uFade"] = 1f - Math.Clamp(fade_progress, 0f, 1f);
            Core.RenderSprite(fade_mat, HudCenter(), new Vector3(Core.DisplayAspectRatio, 1f, 1f), FadeColor, vertex_uniforms);
            return 1;
        }
    }
}