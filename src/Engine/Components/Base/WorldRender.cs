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
        protected uint Priority { get; set; } = 1;
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

        public MaterialData? GetMaterial(int mesh_index)
        {
            if(model == null) return null;
            if(mesh_index < 0 || mesh_index >= materials.Count) return null;
            return materials[mesh_index];
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
                    if(Host.RoomEnabled())
                    {
                        return HandleRender((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    }
                    return HandleRenderDisabled((double)args[0], (List<ShaderData.Uniform>)args[1]);
            }
            return base.ReceiveSignal(signal,args);
        }

        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual uint HandleRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            Debug.Assert(model?.Meshes.Count == materials.Count, "Model rendering with mismatched material(" + materials.Count + ") to mesh(" + model.Meshes.Count + ") count, " + GetType()); // MUST be equal
            // position uniforms
            vertex_uniforms.Add(new("uTransform", Host.GetInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uView", Camera.GetCurrentInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uProjection", Camera.GetCurrentProjectionMatrix()));
            Core.RenderModel( model, materials, vertex_uniforms);
            return 1;
        }

        /// <summary>
        /// Render function run if the component is NOT Visible. Mostly used for long distance LoDs.
        /// </summary>
        public virtual uint HandleRenderDisabled(double delta_time, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 1;
        }
    }
}