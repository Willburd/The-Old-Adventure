using System.Numerics;
using Engine;
using Asset;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that prerenders a skybox model
    /// </summary>
    public class Skybox(Entity host_entity) : WorldRender(host_entity)
    {
        public float sky_rotation_rate = 0.0002f;
        protected float sky_rotation = 0f;
        protected float previous_sky_rotation = 0f;
        protected Quaternion GetInterpolatedSkyboxRotation(double tick_delta)
        {
            return Quaternion.Lerp( Quaternion.CreateFromAxisAngle(Tools.Up, previous_sky_rotation), Quaternion.CreateFromAxisAngle(Tools.Up, sky_rotation), (float)tick_delta);
        }

        protected override uint HandleAssetLoad()
        {
            
            return 1;
        }

        
        protected override uint HandleCreate()
        {
            return 1;
        }

        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.render_priority, Core.Signals.pre_render, Core.Signals.update];
        }
        
        public override uint ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.pre_render:
                    if(Host.RoomEnabled())
                    {
                        return HandleRender((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    }
                    return HandleRenderDisabled((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    
                case Core.Signals.update:
                    previous_sky_rotation = sky_rotation;
                    sky_rotation += sky_rotation_rate;
                    return 1;
            }
            return base.ReceiveSignal(signal,args);
        }

        public override uint HandleRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            if(model == null) return 0;
            if(Camera.WorldCamera == null) return 0;
            // and then call the normal rendering, always at camera pos
            Matrix4x4 sky_matr = Matrix4x4.Identity;
            sky_matr *= Matrix4x4.CreateScale(30f);
            sky_matr *= Matrix4x4.CreateFromQuaternion(Quaternion.Inverse(GetInterpolatedSkyboxRotation(tick_delta)));
            sky_matr *= Matrix4x4.CreateTranslation(Camera.WorldCamera.GetInterpolatedPosition(tick_delta)); 

            vertex_uniforms.Add(new("uTransform", sky_matr));
            vertex_uniforms.Add(new("uView", Camera.GetCurrentInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uProjection", Camera.GetCurrentProjectionMatrix()));
            Core.RenderModel( model, materials, vertex_uniforms);
            return 1;
        }
        
    }
}