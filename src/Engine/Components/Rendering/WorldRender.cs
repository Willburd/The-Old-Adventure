using Engine;
using Assets;
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
        public uint Priority { get; set; } = 20;
        protected ModelData? model;
        protected List<MaterialData> materials = [];

        public void SetModel(ModelData new_model, MaterialData default_material)
        {
            model = new_model;
            ApplyMaterial(default_material, model.Meshes.Count);
        }

        public void SetMaterial(MaterialData apply_mat)
        {
            if (model == null) return;
            ApplyMaterial(apply_mat);
        }

        public void SetMaterial(MaterialData apply_mat, int mesh_index)
        {
            if (model == null) return;
            if (mesh_index < 0 || mesh_index >= materials.Count) return;
            materials[mesh_index] = apply_mat;
        }

        public MaterialData? GetMaterial(int mesh_index)
        {
            if (model == null) return null;
            if (mesh_index < 0 || mesh_index >= materials.Count) return null;
            return materials[mesh_index];
        }

        public MeshData? GetMeshByName(string meshname)
        {
            if (model == null) return null;
            return model.Meshes[model.GetMeshIndex(meshname)];
        }

        public MeshData? GetCollisionMesh(int index)
        {
            if (model == null) return null;
            for (int i = 0; i < model.Meshes.Count; i++)
            {
                if (model.Meshes[i].CollisionTriangles.Count > 0)
                {
                    if (index == 0) return model.Meshes[i];
                    index--;
                }
            }
            return null;
        }

        private void ApplyMaterial(MaterialData apply_mat, int mesh_count = 1)
        {
            if (model == null) return;
            materials = [];
            for (int i = 0; i < mesh_count; i++)
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

        public override uint ReceiveSignal(Core.Signals signal, object?[] args)
        {
            switch (signal)
            {
                case Core.Signals.render_priority:
                    if (!Visible) return 0; // Do not add us to render queue
                    return Priority;

                case Core.Signals.pre_render:
                    return HandlePreRender((double)args[0], (List<ShaderData.Uniform>)args[1]);

                case Core.Signals.render:
                    if (Host.RoomEnabled())
                    {
                        return HandleRender((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    }
                    return HandleRenderDisabled((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    
                case Core.Signals.post_render:
                    return HandlePostRender((double)args[0], (List<ShaderData.Uniform>)args[1]);

                case Core.Signals.hud_render:
                    if (Host.RoomEnabled())
                    {
                        return HandleHudRender((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    }
                    return HandleHudRenderDisabled((double)args[0], (List<ShaderData.Uniform>)args[1]);
            }
            return base.ReceiveSignal(signal, args);
        }

        /// <summary>
        /// PreRender function run if the component is Visible.
        /// </summary>
        public virtual uint HandlePreRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }

        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual uint HandleRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            Debug.Assert(model?.Meshes.Count == materials.Count, "Model rendering with mismatched material(" + materials.Count + ") to mesh(" + model.Meshes.Count + ") count, " + GetType()); // MUST be equal

            CreateBaseUniforms3D(Host.GetInterpolatedViewMatrix(tick_delta), tick_delta, vertex_uniforms);
            Core.RenderModel(model, materials, vertex_uniforms);
            return 1;
        }

        /// <summary>
        /// Render function run if the component is NOT Visible. Mostly used for long distance LoDs.
        /// </summary>
        public virtual uint HandleRenderDisabled(double delta_time, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 1;
        }

        /// <summary>
        /// Post function run if the component is Visible.
        /// </summary>
        public virtual uint HandlePostRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }

        /// <summary>
        /// Hud function run if the component is Visible.
        /// </summary>
        public virtual uint HandleHudRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }

        /// <summary>
        /// HudRender function run if the component is NOT Visible.
        /// </summary>
        public virtual uint HandleHudRenderDisabled(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }



        public static void CreateBaseUniforms3D(Matrix4x4 transform, double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            vertex_uniforms.Add(new("uTransform", transform));
            vertex_uniforms.Add(new("uView", Camera.GetCurrentInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uProjection", Camera.GetCurrentProjectionMatrix()));
        }

        public static void CreateBaseUniforms2D(Matrix4x4 transform, List<ShaderData.Uniform> vertex_uniforms)
        {
            vertex_uniforms.Add(new("uTransform", transform));
            vertex_uniforms.Add(new("uProjection", Matrix4x4.CreateOrthographic(1, 1, 0.0001f, 10000f)));
            vertex_uniforms.Add(new("uView", Matrix4x4.CreateFromQuaternion(Quaternion.Identity) * Matrix4x4.CreateTranslation(Tools.Forward)));
        }
    }
}