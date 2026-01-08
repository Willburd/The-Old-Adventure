using Engine;
using Rendering;
using System.Diagnostics;
using System.Numerics;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that handles the basics of rendering a 3D model's meshes, using materials assigned to each mesh.
    /// </summary>
    public class WorldRender(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Visible { get; set; } = true;
        private uint Priority { get; set; } = 1;
        protected ModelData? model;
        protected List<MaterialData> materials = [];

        public void SetModel(ModelData new_model, MaterialData default_material)
        {
            model = new_model;
            ApplyMaterial(default_material, model.Meshes.Count);
        }

        public void SetMaterial(MaterialData apply_mat, int mesh_index)
        {
            if(model == null) return;
            if(mesh_index < 0 || mesh_index >= materials.Count) return;
            materials[mesh_index] = apply_mat;
        }

        
        public MeshData? GetMeshByName(string meshname)
        {
            if(model == null) return null;
            return model.Meshes[model.GetMeshIndex(meshname)];
        }
        
        public MeshData? GetCollisionMesh(int index)
        {
            if(model == null) return null;
            for(int i = 0; i < model.Meshes.Count; i++)
            {
                if(model.Meshes[i].CollisionTriangles.Count > 0)
                {
                    if(index == 0) return model.Meshes[i];
                    index--;
                }
            }
            return null;
        }

        private void ApplyMaterial(MaterialData apply_mat, int mesh_count = 1)
        {
            if(model == null) return;
            for(int i = 0; i < mesh_count; i++)
            {
                materials.Add(apply_mat);
            }
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> PrepareSignals()
        {
            return [Core.Signals.render_priority, Core.Signals.render];
        }

        public override uint ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.render_priority:
                    if(!Visible) return 0; // Do not add us to render queue
                    return Priority;

                case Core.Signals.render:
                    if(Host.Enabled)
                    {
                        return HandleRender((double)args[0]);
                    }
                    return HandleRenderDisabled((double)args[0]);

            }
            return base.ReceiveSignal(signal,args);
        }


        public const int max_lights = 16; // Must match in shader

        /// <summary>
        /// Construct vertex shader uniforms for light data.
        /// </summary>
        private void BuildLightData(List<KeyValuePair<string,object>> vertex_uniforms, double tick_delta)
        {
            // environment lights
            Vector4 environ_pos = new Vector4(0f,0f,0f,32);
            Vector4 environ_color = Vector4.One;

            // Vertex lighting data
            Vector4[] light_pos = new Vector4[WorldRender.max_lights]; // pos + radius
            Vector4[] light_col = new Vector4[WorldRender.max_lights]; // color + alpha
            light_pos[0] = environ_pos;
            light_col[0] = environ_color;

            vertex_uniforms.Add(new("uLightPositions", light_pos)); 
            vertex_uniforms.Add(new("uLightColors", light_col)); 
            vertex_uniforms.Add(new("uLightCount", 1 )); // Number of lights, not max lights
        }

        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual uint HandleRender(double tick_delta)
        {
            Debug.Assert(model?.Meshes.Count == materials.Count, "Model rendering with mismatched material(" + materials.Count + ") to mesh(" + model.Meshes.Count + ") count, " + GetType()); // MUST be equal
            
            List<KeyValuePair<string,object>> vertex_uniforms = [];
            // position uniforms
            vertex_uniforms.Add(new("uTransform", Host.GetInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uView", Camera.GetCurrentInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uProjection", Camera.GetCurrentProjectionMatrix()));
            BuildLightData(vertex_uniforms, tick_delta);
            Core.RenderModel( model, materials, vertex_uniforms);
            return 1;
        }

        /// <summary>
        /// Render function run if the component is NOT Visible. Mostly used for long distance LoDs.
        /// </summary>
        public virtual uint HandleRenderDisabled(double delta_time)
        {
            return 1;
        }
    }
}