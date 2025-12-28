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
            Host.Position += new Vector3(0f,-0.5f,0f);
            Host.Rotation = Quaternion.CreateFromAxisAngle(Tools.Up, 15f);
            Host.Rotation = Quaternion.CreateFromAxisAngle(Tools.Forward, 75f);
            Host.Scale *= 1f;
            Host.SnapTransform();

            spin_speed = Tools.RandRange(0.01f,0.03f);

            return 1;
        }

        protected override uint HandleUpdate()
        {
            float speed = 0.01f;
            //curloc?.Position += new Vector3( Tools.RandRange(-speed,speed), Tools.RandRange(-speed,speed), Tools.RandRange(-speed,speed));
            Host.Rotation *= Quaternion.CreateFromAxisAngle(Tools.Up, spin_speed);
            Host.SnapTransform();
            
            return 1;
        }
    }
}