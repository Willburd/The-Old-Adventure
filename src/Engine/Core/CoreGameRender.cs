using Assets;
using Silk.NET.OpenGL;
using Silk.NET.Windowing;
using System.Drawing;
using System.Numerics;
using EntComponents;
using Rendering;

namespace Engine
{
    public partial class Core
    {
        public static GL OpenGLContext { get; private set; }

        /// <summary>
        /// Number of frames per second for rendering.
        /// </summary>
        private static double FPS { get; set; } = 60;

        /// <summary>
        /// The threshold needed for the delta_time accumulator to trigger a frame render.
        /// </summary>
        private static double FpsTickInterval { get { return 1.0 / FPS; } }
        private static double game_fps_accumulator = 0;

        /// <summary>
        /// Skips delta_time check for rendering frames, forcing a frame to be renderer as soon as possible. Used when changing scenes for example.
        /// </summary>
        private static bool RequestRender { get; set; }

        /// <summary>
        /// Number of renderer frames since launch.
        /// </summary>
        public static long ElapsedGameFrames { get; private set; }

        /// <summary>
        /// Percent difference from the previous game tick, to the next gametick. Used to do "inbetween" frames during rendering. 
        /// </summary>
        public static double GameTickDelta { get { return game_tick_accumulator % GameTickInterval / GameTickInterval; } }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Game Rendering
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Configures GL settings, can be overriden to replace or extend configuration for your own adventure.
        /// </summary>
        public virtual void ConfigureGL()
        {
            // Default draw control
            OpenGLContext.ClearColor(Color.CornflowerBlue);

            // Depth control
            OpenGLContext.Enable(EnableCap.DepthTest);
            OpenGLContext.DepthFunc(DepthFunction.Less);

            // Blending
            OpenGLContext.Enable(EnableCap.Blend);

            // Backface culling
            OpenGLContext.Enable(EnableCap.CullFace);
            OpenGLContext.CullFace(GLEnum.Back);

            // GLTF format
            OpenGLContext.FrontFace(FrontFaceDirection.Ccw);
        }

        /// <summary>
        /// Create framebuffers for complex rendering tasks and down/upsampling.
        /// </summary>
        public virtual void CreateFrameBuffers()
        {
            uint render_size = InternalRenderResolutionHeight;
            if (InternalRenderScale > 0f) render_size = (uint)(DisplayHeight * InternalRenderScale);
            FrameBuffer_Game = new Rendering.FrameBufferContainer(GetAspectWidth(render_size), render_size);
        }

        /// <summary>
        /// Handles rendering the game at the desired interval, called by the window itself.
        /// </summary>
        private static void HandleWindowRender(double deltaTime)
        {
            if (shutting_down) return;

            game_fps_accumulator += deltaTime;
            if (game_fps_accumulator >= FpsTickInterval || RequestRender)
            {
                ElapsedGameFrames++;
                // We're effectively lerping between the previous draw and the new draw based on how far the gametick has progressed
                singleton.RenderTick(GameTickDelta);
                game_fps_accumulator %= FpsTickInterval;
                RequestRender = false;
                WindowContext.SwapBuffers();
            }
        }

        /// <summary>
        /// Render tick, fired at the game's framerate. Sends a render signals to all entities depending on their enabled state.
        /// </summary>
        private void RenderTick(double tick_delta)
        {
            // Clear screen
            FrameBuffer_Game.BindFrameBuffer();
            List<ShaderData.Uniform> vertex_uniforms = [];
            OpenGLContext.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            // Draw radius
            Vector3 world_load_position = Vector3.Zero;
            if (Camera.WorldCamera != null) world_load_position = Camera.WorldCamera.Position;

            float frustum_dot_product_limit = -0.35f; // Very generous
            Vector3 camera_vector = Tools.Forward;
            if (Camera.WorldCamera != null) camera_vector = Vector3.Transform(Tools.Forward, Camera.WorldCamera.Rotation);

            // Assemble a list in order of priority.
            SortedList<uint, List<Entity>> render_queue = []; // Stores lists of entities in each priority, as their creaiton order is all that matters if they are in the same queue anyway
            ApplyPrerenderEnvironmentUniforms(vertex_uniforms, tick_delta);
            OnPreRenderTick();
            foreach (Entity check in Entity.EntityList)
            {
                // Check the entity for a render priority. We only draw if we have one, as that means we have a component that wants to draw!
                uint priority = check.SendSignal(Signals.render_priority, tick_delta);
                if (priority == 0) continue; // Not visible if no component responds.

                // Check vis culling
                float dist = Vector3.Distance(world_load_position, check.Position);
                if (dist > world_load_radius) continue;
                if (dist > check.MinimumRenderDistance)
                {
                    float dot_prod = Vector3.Dot(Tools.DirVector(check.Position, world_load_position), camera_vector);
                    if (dot_prod >= frustum_dot_product_limit) continue; // Nothing behind us
                }

                // perform prerender while we're here.
                check.SendSignal(Signals.pre_render, tick_delta, vertex_uniforms);
                // Add to queue for all of the following render loops, instead of checking every entity for each one! We only store the ones that replied with a draw priority!
                if (!render_queue.ContainsKey(priority)) render_queue.Add(priority, []);
                render_queue[priority].Add(check);
            }

            // Primary rendering
            OpenGLContext.Clear(ClearBufferMask.DepthBufferBit);
            vertex_uniforms.Clear();
            ApplyEnvironmentUniforms(vertex_uniforms, tick_delta);
            OnRenderTick();
            foreach ((uint key, List<Entity> draw_list) in render_queue)
            {
                foreach (Entity draw in draw_list)
                {
                    draw.SendSignal(Signals.render, tick_delta, vertex_uniforms);
                }
            }

            // Late rendering
            OpenGLContext.Clear(ClearBufferMask.DepthBufferBit);
            OnPostRenderTick();
            foreach ((uint key, List<Entity> draw_list) in render_queue)
            {
                foreach (Entity draw in draw_list)
                {
                    draw.SendSignal(Signals.post_render, tick_delta);
                }
            }

            // Hud rendering
            OpenGLContext.Clear(ClearBufferMask.DepthBufferBit);
            OnRenderHudTick();
            foreach ((uint key, List<Entity> draw_list) in render_queue)
            {
                foreach (Entity draw in draw_list)
                {
                    draw.SendSignal(Signals.hud_render, tick_delta);
                }
            }


            // Render buffer
            FrameBufferContainer.BindDefaultFrameBuffer();
            OpenGLContext.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
            FrameBuffer_Game.Render(tick_delta);
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Asset Rendering
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        public static void RenderModel(ModelData model, List<MaterialData> materials, List<ShaderData.Uniform> vertex_uniforms)
        {
            // Render each mesh!
            int mesh_index = 0;
            foreach (var mesh in model.Meshes)
            {
                RenderMesh(mesh, materials[mesh_index], vertex_uniforms);
                mesh_index++;
            }
        }

        public static void RenderMesh(MeshData mesh, MaterialData mat_data, List<ShaderData.Uniform> vertex_uniforms)
        {
            // Check for collision drawing
            if (mesh.RawName == "col.001")
            {
                if (!draw_collisions) return;
                mat_data = collision_draw_material; // Use our collision shader
            }

            // Bind the VBOs
            mesh.Bind();

            // Set the blending mode
            mat_data.UseBlendMode();

            // Each mesh can use a different material, and that also means shader!
            ShaderData shader = mat_data.Shader;
            shader.Use();
            foreach (ShaderData.Uniform vertuni in vertex_uniforms)
            {
                shader.SetUniform(vertuni.key, vertuni.value, vertuni.count);
            }

            // Bind textures to texunits
            int tex_unit_id = 0;
            foreach (TextureData tex in mat_data.Textures)
            {
                tex.Bind(tex_unit_id);
                tex_unit_id++;
            }

            // Apply shader uniforms
            foreach (ShaderData.Uniform matuni in mat_data.Uniforms)
            {
                shader.SetUniform(matuni.key, matuni.value, matuni.count);
            }

            // Draw mesh
            OpenGLContext.DrawArrays(PrimitiveType.Triangles, 0, (uint)mesh.Indices.Length);
        }

        public static void RenderSprite(List<ShaderData.Uniform> vertex_uniforms)
        {
            RenderSprite(sprite2d_material, vertex_uniforms);
        }

        public static void RenderSprite(MaterialData mat_data, List<ShaderData.Uniform> vertex_uniforms)
        {
            // Bind the VBOs
            MeshData mesh = sprite2d_model.Meshes[0];
            mesh.Bind();

            // Set the blending mode
            mat_data.UseBlendMode();

            // Each mesh can use a different material, and that also means shader!
            ShaderData shader = mat_data.Shader;
            shader.Use();
            foreach (ShaderData.Uniform vertuni in vertex_uniforms)
            {
                shader.SetUniform(vertuni.key, vertuni.value, vertuni.count);
            }

            // Bind textures to texunits
            int tex_unit_id = 0;
            foreach (TextureData tex in mat_data.Textures)
            {
                tex.Bind(tex_unit_id);
                tex_unit_id++;
            }

            // Apply shader uniforms
            foreach (ShaderData.Uniform matuni in mat_data.Uniforms)
            {
                shader.SetUniform(matuni.key, matuni.value, matuni.count);
            }

            // Draw mesh
            OpenGLContext.DrawArrays(PrimitiveType.Triangles, 0, (uint)mesh.Indices.Length);
        }

        public static void RenderSprite(FrameBufferContainer fbo, List<ShaderData.Uniform> vertex_uniforms)
        {
            // Bind the VBOs
            MeshData mesh = sprite2d_model.Meshes[0];
            mesh.Bind();

            // Set the blending mode
            OpenGLContext.BlendFunc(BlendingFactor.SrcAlpha, BlendingFactor.OneMinusSrcAlpha);

            // Each mesh can use a different material, and that also means shader!
            ShaderData shader = sprite2d_material.Shader;
            shader.Use();
            foreach (ShaderData.Uniform vertuni in vertex_uniforms)
            {
                shader.SetUniform(vertuni.key, vertuni.value, vertuni.count);
            }

            // Bind textures to texunits
            fbo.BindTexture();

            // Apply shader uniforms
            foreach (ShaderData.Uniform matuni in sprite2d_material.Uniforms)
            {
                shader.SetUniform(matuni.key, matuni.value, matuni.count);
            }

            // Draw mesh
            OpenGLContext.DrawArrays(PrimitiveType.Triangles, 0, (uint)mesh.Indices.Length);
        }
    }
}