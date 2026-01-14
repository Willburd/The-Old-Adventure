using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that applies physics to the entity's transform.
    /// </summary>
    public class PhysicsBody(Entity host_entity) : EntComponent(host_entity)
    {
        public Vector3 Velocity { get; set; } = new Vector3();
        public const float default_friction = 0.01f;
        public float FlatFriction { get; set; } = default_friction;
        public float SphereFriction { get; set; } = 0f;
        public const float default_gravity = 0.01f;
        public Vector3 Gravity { get; set; } = new Vector3(0f, -default_gravity, 0f);
        public bool HasGravity { get; set; } = true;

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.apply_physics];
        }
        public override uint ReceiveSignal(Core.Signals signal, object?[] args)
        {
            switch (signal)
            {
                case Core.Signals.apply_physics:
                    if (!ActiveUpdate) return 0;
                    return HandlePhysicsUpdate();
            }
            return base.ReceiveSignal(signal, args);
        }

        private uint HandlePhysicsUpdate()
        {
            if (HasGravity) Velocity += Gravity;
            if (FlatFriction > 0f) Velocity = Tools.DecelerateFlat(Velocity, FlatFriction);
            if (SphereFriction > 0f) Velocity = Tools.Decelerate(Velocity, SphereFriction);
            Host.Position += Velocity;

            return 1;
        }
    }
}