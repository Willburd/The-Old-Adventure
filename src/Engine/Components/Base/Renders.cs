using Engine;
using Rendering;
using System.Diagnostics;
using System.Numerics;
using Silk.NET.Maths;
using Silk.NET.OpenGL;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that handles the basics of rendering a 3D model's meshes, using materials assigned to each mesh. Intended to be overriden.
    /// </summary>
    public class Renders(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Visible { get; set; } = true;
        private uint Priority { get; set; } = 1;
        protected Rendering.ModelData? model;
        protected List<Rendering.MaterialData> materials = [];

        public void ApplyMaterial(MaterialData apply_mat, int mesh_count = 1)
        {
            for(int i = 0; i < mesh_count; i++)
            {
                materials.Add(apply_mat);
            }
        }

        public void ChangeMaterial(MaterialData apply_mat, int mesh_index)
        {
            materials[mesh_index] = apply_mat;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.create, Core.Signals.cache_components, Core.Signals.render_priority, Core.Signals.render, Core.Signals.hud_render];
        }

        public override uint ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.render_priority:
                    if(!Visible) return 0; // Do not add us to render queue
                    return Priority;

                case Core.Signals.pre_render:
                    if(Host.Enabled)
                    {
                        return HandlePreRender((double)args[0]);
                    }
                    return 0;

                case Core.Signals.render:
                    if(Host.Enabled)
                    {
                        return HandleRender((double)args[0]);
                    }
                    return HandleRenderDisabled((double)args[0]);
                    
                case Core.Signals.hud_render:
                    if(Host.Enabled)
                    {
                        return HandleHudRender((double)args[0]);
                    }
                    return 0;

            }
            return base.ReceiveSignal(signal,args);
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual uint HandlePreRender(double delta_time)
        {
            return 1;
        }

        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual uint HandleRender(double tick_delta)
        {
            Debug.Assert(model?.Meshes.Count == materials.Count, "Model rendering with mismatched material(" + materials.Count + ") to mesh(" + model.Meshes.Count + ") count, " + GetType()); // MUST be equal
            
            // Get the transform if we have one, and apply it to the model's meshs.
            Matrix4x4 model_matrix = Host.GetInterpolatedViewMatrix(tick_delta);

            // Render each mesh!
            int mesh_index = 0;
            foreach (var mesh in model.Meshes)
            {
                // Bind the VBOs
                mesh.Bind();

                // Each mesh can use a different material, and that also means shader!
                MaterialData mat_data = materials[mesh_index];
                ShaderData shader = mat_data.Shader;
                shader.Use(); 
                shader.SetUniform("uTransform", model_matrix);

                // Bind textures to texunits
                int tex_unit_id = 0;
                foreach(TextureData tex in materials[mesh_index].Textures)
                {
                    tex.Bind((TextureUnit)tex_unit_id);
                    tex_unit_id++;
                }
                
                // Apply shader uniforms
                foreach(MaterialUniformData matuni in mat_data.Uniforms)
                {
                    shader.SetUniform(matuni.set_uniform, matuni.shader_uni_value);
                }

                // Draw mesh
                Core.OpenGLContext.DrawArrays( PrimitiveType.Triangles, 0, (uint)mesh.Indices.Length);
                mesh_index++;
            }

            return 1;
        }

        /// <summary>
        /// Render function run if the component is NOT Visible. Mostly used for long distance LoDs.
        /// </summary>
        public virtual uint HandleRenderDisabled(double delta_time)
        {
            return 1;
        }
        
        /// <summary>
        /// Render function run after all others, meant for drawing the hud over the the render canvas.
        /// </summary>
        public virtual uint HandleHudRender(double delta_time)
        {
            return 1;
        }
    }
}