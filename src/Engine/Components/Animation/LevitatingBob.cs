using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that rotates the transform of it's host, and nothing else.
    /// </summary>
    public class LevitatingBob(Entity host_entity) : EntComponent(host_entity)
    {
        private float animation_bob = 0f;
        public float BobSpeed { get; set; } = 0.03f;
        public Vector3 BobIntensity { get; set; } = new Vector3(0.1f, 0.1f, 0.1f);

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.update];
        }

        protected override uint HandleUpdate()
        {
            animation_bob += BobSpeed;
            float bob_offset = Host.UniqueSeed * 0.001f * Math.Sign(MathF.Sin(Host.UniqueSeed));
            if (bob_offset == 0) bob_offset = Host.UniqueSeed;
            // Commence the jigglin!
            Host.Position = Host.StartPosition + (new Vector3(MathF.Sin(animation_bob + bob_offset), MathF.Cos(animation_bob - (bob_offset * -1.1f)), MathF.Sin(animation_bob + (bob_offset * 1.12f))) * BobIntensity);
            return 1;
        }
    }
}