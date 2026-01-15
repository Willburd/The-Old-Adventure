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

        public static bool draw_collisions = false;

        /// <summary>
        /// If set larger than 0f, will resize the game FBO to match the window's size at that scale. Otherwise if 0, it will used the fixed height in InternalRenderResolutionHeight. Set to 1f to match window resolution.
        /// </summary>
        public static float InternalRenderScale = 0f;

        /// <summary>
        /// Height in pixels of the internal render resolution. For downscaled pixely rendering of older-era themed games. Only used it InternalRenderScale is 0f.
        /// </summary>
        public static uint InternalRenderResolutionHeight { get; protected set; } = 144;

        /// <summary>
        /// Framebuffer object used to render the game. It is rendered to the window's buffer and stretched to fit in order to down or upsample the game view.
        /// Intentionally uses a fixed pixel height with a dynamic width to keep hud elements from changing scale. A purely style choice to match older era games.
        /// </summary>
        public static Rendering.FrameBufferContainer FrameBuffer_Game { get; protected set; }

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
            //FrameBuffer_Game.BindFrameBuffer();
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
        public const int max_lights = 16; // Must match in shader

        /// <summary>
        /// Construct vertex shader uniforms for light data.
        /// </summary>
        private static void ApplyPrerenderEnvironmentUniforms(List<ShaderData.Uniform> vertex_uniforms, double tick_delta)
        {
            Vector4[] light_pos = new Vector4[max_lights];
            Vector4[] light_col = new Vector4[max_lights];
            light_pos[0] = new(0f, 0f, 0f, float.PositiveInfinity);
            light_col[0] = new(1f, 1f, 1f, 1f);
            ApplyVertexUniforms(vertex_uniforms, light_pos, light_col, 1, Vector4.Zero, float.PositiveInfinity);
        }

        /// <summary>
        /// Construct vertex shader uniforms for light data.
        /// </summary>
        private static void ApplyEnvironmentUniforms(List<ShaderData.Uniform> vertex_uniforms, double tick_delta)
        {
            // Vertex lighting data
            Vector4[] light_pos = new Vector4[max_lights];
            Vector4[] light_col = new Vector4[max_lights];

            // Environment
            float fog_distance = 1000f;
            Vector4 fog_color = Tools.ColorToVector(Color.CornflowerBlue);
            light_pos[0] = new(0f, 0f, 0f, float.PositiveInfinity);
            if (Room.loaded_rooms.Count == 0)
            {
                // Default lighting
                light_col[0] = new(1f, 1f, 1f, 1f);
            }
            else if (Room.loaded_rooms.Count == 1)
            {
                // Single environment, apply just it.
                light_col[0] = Room.loaded_rooms[0].Environment.AmbientLight;
                fog_color = Room.loaded_rooms[0].Environment.FogColor;
                fog_distance = Room.loaded_rooms[0].Environment.FogDistance;
            }
            else
            {
                // Multiple environments are blended together
                Vector3 camera_pos = (Camera.WorldCamera?.Position) ?? new Vector3();
                Vector4 blended_environment_light = new();
                Vector4 blended_environment_fog = new();
                float blended_environment_fog_distance = 0f;
                float total_distance = 0f;
                foreach (Room room in Room.loaded_rooms)
                {
                    total_distance += Vector3.Distance(camera_pos, room.Position);
                }
                foreach (Room room in Room.loaded_rooms)
                {
                    float distance_merge = Vector3.Distance(camera_pos, room.Position) / total_distance;
                    if (room.Environment != null)
                    {
                        blended_environment_light += room.Environment.AmbientLight * distance_merge;
                        blended_environment_fog += room.Environment.FogColor * distance_merge;
                        blended_environment_fog_distance += room.Environment.FogDistance * distance_merge;
                    }
                }
                light_col[0] = blended_environment_light;
                fog_color = blended_environment_fog;
                fog_distance = blended_environment_fog_distance;
            }

            // Dynamic lights
            int light_count = 1;
            foreach (Light light in EntComponent.GetAllOfType(typeof(Light)).Cast<Light>())
            {
                light_pos[light_count] = new(light.OffsetPos.X, light.OffsetPos.Y, light.OffsetPos.Z, light.Radius);
                light_col[light_count] = light.Color;
                light_count++;
                if (light_count >= max_lights) break;
            }

            // Assemble uniforms
            ApplyVertexUniforms(vertex_uniforms, light_pos, light_col, light_count, fog_color, fog_distance);
        }

        private static void ApplyVertexUniforms(List<ShaderData.Uniform> vertex_uniforms, Vector4[] light_pos_array, Vector4[] light_color_array, int light_count, Vector4 fog_color, float fog_distance)
        {
            vertex_uniforms.Add(new("uLightPositions", light_pos_array));
            vertex_uniforms.Add(new("uLightColors", light_color_array));
            vertex_uniforms.Add(new("uLightCount", light_count)); // Number of lights, not max lights
            vertex_uniforms.Add(new("uFogColor", fog_color));
            vertex_uniforms.Add(new("uFogDistance", fog_distance));
        }

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
            fbo.BindTexture(0);

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