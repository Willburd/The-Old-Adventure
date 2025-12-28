using System.Numerics;
using Engine;

namespace EntComponents
{
    public class TestActorBehavior(Entity host_entity) : Renders(host_entity)
    {
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.create, Core.Signals.cache_components, Core.Signals.update, Core.Signals.render_priority, Core.Signals.render];
        }

        float spin_speed = 0;

        protected override uint HandleCreate()
        {
            // Apply assets
            model = AssetLoader.ModelAssetGet("sign");
            materials.Add(AssetLoader.MaterialAssetGet( "sign_wood")); // sign
            materials.Add(AssetLoader.MaterialAssetGet( "sign_face")); // face
            
            // Set location
            Host.Scale *= Tools.RandRange(0.5f,1.5f);
            Host.SnapTransform();

            spin_speed = Tools.RandRange(0.1f,0.6f);

            return 1;
        }

        protected override uint HandleUpdate()
        {
            float speed = 0.01f;
            //Host.Position += new Vector3( Tools.RandRange(-speed,speed), Tools.RandRange(-speed,speed), Tools.RandRange(-speed,speed));
            Host.Rotation *= Quaternion.CreateFromAxisAngle(Tools.Up, spin_speed);
            return 1;
        }
    }
}