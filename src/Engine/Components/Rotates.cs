using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that rotates the transform of it's host, and nothing else.
    /// </summary>
    public class Rotates(Entity host_entity) : EntComponent(host_entity)
    {
        public Vector3 rotation = Vector3.Zero;

        public Actor? sync_to_actor = null;

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.update,Core.Signals.post_update];
        }

        protected override uint HandleUpdate()
        {
            Host.Rotation *= Quaternion.CreateFromYawPitchRoll(rotation.Y, rotation.X, rotation.Z);
            return 1;
        }

        protected override uint HandlePostUpdate()
        {
            if (sync_to_actor != null) Host.Rotation = sync_to_actor.Rotation;
            return 1;
        }
    }
}