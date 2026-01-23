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
                    if (Host.RoomEnabled())
                    {
                        return HandlePreRender((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    }
                    return HandlePreRenderDisabled((double)args[0], (List<ShaderData.Uniform>)args[1]);

                case Core.Signals.render:
                    if (Host.RoomEnabled())
                    {
                        return HandleRender((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    }
                    return HandleRenderDisabled((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    
                case Core.Signals.post_render:
                    if (Host.RoomEnabled())
                    {
                        HandlePostRender((double)args[0], (List<ShaderData.Uniform>)args[1]);
                    }
                    return HandlePostRenderDisabled((double)args[0], (List<ShaderData.Uniform>)args[1]);

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
        protected virtual uint HandlePreRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }
        
        /// <summary>
        /// PreRender function run if the component is NOT Visible.
        /// </summary>
        protected virtual uint HandlePreRenderDisabled(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }

        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        protected virtual uint HandleRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            Debug.Assert(model?.Meshes.Count == materials.Count, "Model rendering with mismatched material(" + materials.Count + ") to mesh(" + model.Meshes.Count + ") count, " + GetType()); // MUST be equal

            CreateBaseUniforms3D(Host.GetInterpolatedViewMatrix(tick_delta), tick_delta, vertex_uniforms);
            Core.RenderModel(model, materials, vertex_uniforms);
            return 1;
        }

        /// <summary>
        /// Render function run if the component is NOT Visible. Mostly used for long distance LoDs.
        /// </summary>
        protected virtual uint HandleRenderDisabled(double delta_time, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 1;
        }

        /// <summary>
        /// Post function run if the component is Visible.
        /// </summary>
        protected virtual uint HandlePostRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }

        /// <summary>
        /// Post function run if the component is NOT Visible.
        /// </summary>
        protected virtual uint HandlePostRenderDisabled(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }

        /// <summary>
        /// Hud function run if the component is Visible.
        /// </summary>
        protected virtual uint HandleHudRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }

        /// <summary>
        /// HudRender function run if the component is NOT Visible.
        /// </summary>
        protected virtual uint HandleHudRenderDisabled(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            return 0;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Hud alignment helpers
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static Vector3 HudTopLeft()
        {
            return new Vector3(-0.5f, 0.5f, 0f) * new Vector3(Core.DisplayAspectRatio, 1f, 1f);
        }

        public static Vector3 HudTopRight()
        {
            return new Vector3(0.5f, 0.5f, 0f) * new Vector3(Core.DisplayAspectRatio, 1f, 1f);
        }

        public static Vector3 HudBottomLeft()
        {
            return new Vector3(-0.5f, -0.5f, 0f) * new Vector3(Core.DisplayAspectRatio, 1f, 1f);
        }

        public static Vector3 HudBottomRight()
        {
            return new Vector3(0.5f, -0.5f, 0f) * new Vector3(Core.DisplayAspectRatio, 1f, 1f);
        }

        public static Vector3 HudCenter()
        {
            return Vector3.Zero;
        }
        
        public static Vector3 HudLerp(float hor, float ver)
        {
            Vector3 pos = Vector3.Lerp(HudTopLeft(), HudTopRight(), hor);
            pos.Y = Vector3.Lerp(HudBottomLeft(), HudTopLeft(), ver).Y;
            return pos;
        }

        public static Vector3 HudUnit()
        {
            return new Vector3(Core.DisplayAspectRatio, 1f, 1f) * 0.1f;
        }

        public static Vector3 HudScale()
        {
            return new Vector3(Core.DisplayAspectRatio, 1f, 1f);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Default uniform shader setup
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// 3D camera projection uniforms.
        /// </summary>
        public static void CreateBaseUniforms3D(Matrix4x4 transform, double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            vertex_uniforms.Add(new("uTransform", transform));
            vertex_uniforms.Add(new("uView", Camera.GetCurrentInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uProjection", Camera.GetCurrentProjectionMatrix()));
        }

        /// <summary>
        /// 2D hud uniforms.
        /// </summary>
        public static void CreateBaseUniforms2D(Matrix4x4 transform, List<ShaderData.Uniform> vertex_uniforms)
        {
            vertex_uniforms.Add(new("uTransform", transform));
            vertex_uniforms.Add(new("uProjection", Matrix4x4.CreateOrthographic(1, 1, 0.0001f, 10000f)));
            vertex_uniforms.Add(new("uView", Matrix4x4.CreateFromQuaternion(Quaternion.Identity) * Matrix4x4.CreateTranslation(Tools.Forward)));
        }

        /// <summary>
        /// 2D sprite offset and scale uniforms. Allows cutout sections of a texture atlas to be used.
        /// </summary>
        public static void CreateSprite2DUniforms(Vector2 cutout_pos, Vector2 cutout_size, Vector3 draw_pos, Vector3 draw_scale, Vector3 draw_color, List<ShaderData.Uniform> vertex_uniforms)
        {
            vertex_uniforms.Add(new("uSpritePos", cutout_pos));
            vertex_uniforms.Add(new("uSpriteSize", cutout_size));
            vertex_uniforms.Add(new("uDrawOffset", Matrix4x4.CreateScale(draw_scale) * Matrix4x4.CreateTranslation(draw_pos + new Vector3(0f, 0f, Core.SpriteRenderDepthOffset))));
            vertex_uniforms.Add(new("uDrawColor", draw_color));
        }
    }
}