using Engine;
using System.Numerics;

namespace EntComponents
{
    public class BillboardRender : WorldRender
    {
        public BillboardRender(Entity host_entity) : base(host_entity)
        {
            model = Core.sprite2d_model;
        }
        
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.render_priority, Core.Signals.render, Core.Signals.post_update];
        }

        protected override uint HandlePostUpdate()
        {
            // Face the camera directly
            Vector3 camera_pos = Vector3.Zero;
            if (Camera.WorldCamera != null) camera_pos = Camera.WorldCamera.Position;
            Host.Rotation = Tools.LookAtLockedZ(Host.Position, camera_pos);
            return 1;
        }
    }
}