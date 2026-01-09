using Rendering;
using Silk.NET.OpenGL;
using Silk.NET.Windowing;
using System.Drawing;
using System.Numerics;
using EntComponents;

namespace Engine
{
    public partial class Core
    {
        public static GL OpenGLContext {get; private set;}

        public static bool draw_collisions = false;

        /// <summary>
        /// Number of frames per second for rendering.
        /// </summary>
        private static double FPS {get; set;} = 60;

        /// <summary>
        /// The threshold needed for the delta_time accumulator to trigger a frame render.
        /// </summary>
        private static double FpsTickInterval {get{ return 1.0 /  FPS; }}
        private static double game_fps_accumulator = 0;
        
        /// <summary>
        /// Skips delta_time check for rendering frames, forcing a frame to be renderer as soon as possible. Used when changing scenes for example.
        /// </summary>
        private static bool RequestRender {get; set;}

        /// <summary>
        /// Number of renderer frames since launch.
        /// </summary>
        public static long ElapsedGameFrames {get; private set;}
        
        /// <summary>
        /// Percent difference from the previous game tick, to the next gametick. Used to do "inbetween" frames during rendering. 
        /// </summary>
        public static double GameTickDelta {get{ return game_tick_accumulator % GameTickInterval / GameTickInterval; }}

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Game Rendering
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        /// <summary>
        /// Configures GL settings, can be overriden to replace or extend configuration for your own adventure.
        /// </summary>
        public virtual void ConfigureGL()
        {
            // Default draw control
            OpenGLContext?.ClearColor(Color.CornflowerBlue);
            
            // Depth control
            OpenGLContext?.Enable(EnableCap.DepthTest);
            OpenGLContext?.DepthFunc(DepthFunction.Less);

            // Blending
            OpenGLContext?.Enable(EnableCap.Blend);

            // Backface culling
            OpenGLContext?.Enable(EnableCap.CullFace);
            OpenGLContext?.CullFace(GLEnum.Back);

            // GLTF format
            OpenGLContext?.FrontFace(FrontFaceDirection.Ccw); 
        }

        /// <summary>
        /// Handles rendering the game at the desired interval, called by the window itself.
        /// </summary>
        private static void HandleWindowRender(double deltaTime)
        {
            game_fps_accumulator += deltaTime;
            if(game_fps_accumulator >= FpsTickInterval || RequestRender)
            {
                ElapsedGameFrames++;
                // We're effectively lerping between the previous draw and the new draw based on how far the gametick has progressed
                singleton?.RenderTick(GameTickDelta); 
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
            OpenGLContext?.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

            // Assemble a list in order of priority.
            SortedList<uint,List<Entity>> render_queue = []; // Stores lists of entities in each priority, as their creaiton order is all that matters if they are in the same queue anyway
            foreach(Entity check in Entity.EntityList)
            {
                // Check the entity for a render priority. We only draw if we have one, as that means we have a component that wants to draw!
                uint priority = check.SendSignal(Signals.render_priority, tick_delta);
                if(priority == 0) continue; // Not visible if no component responds.
                check.SendSignal(Signals.pre_render, tick_delta); // perform prerender while we're here.
                
                // Add to queue for all of the following render loops, instead of checking every entity for each one! We only store the ones that replied with a draw priority!
                if(!render_queue.ContainsKey(priority)) render_queue.Add(priority, []);
                render_queue[priority].Add(check);
            }
            OnPreRenderTick();

            // Primary rendering
            List<ShaderData.Uniform> vertex_uniforms = [];
            BuildLightData(vertex_uniforms, tick_delta);
            foreach((uint key, List<Entity> draw_list) in render_queue)
            {
                foreach(Entity draw in draw_list)
                {
                    draw.SendSignal(Signals.render, tick_delta, vertex_uniforms);
                }
            }
            OnRenderTick();
            
            // Late rendering
            foreach((uint key, List<Entity> draw_list) in render_queue)
            {
                foreach(Entity draw in draw_list)
                {
                    draw.SendSignal(Signals.post_render, tick_delta);
                }
            }
            OnPostRenderTick();

            // Hud rendering
            foreach((uint key, List<Entity> draw_list) in render_queue)
            {
                foreach(Entity draw in draw_list)
                {
                    draw.SendSignal(Signals.hud_render, tick_delta);
                }
            }
            OnRenderHudTick();
        }
        public const int max_lights = 16; // Must match in shader

        /// <summary>
        /// Construct vertex shader uniforms for light data.
        /// </summary>
        private static void BuildLightData(List<ShaderData.Uniform> vertex_uniforms, double tick_delta)
        {
            // Vertex lighting data
            int light_count = 0;
            Vector4[] light_pos = new Vector4[max_lights];
            Vector4[] light_col = new Vector4[max_lights];

            // Environment
            light_pos[light_count] = new(0f,0f,0f,float.PositiveInfinity);
            light_col[light_count] = new(1f,1f,1f, 0.5f + (MathF.Sin((float)Core.ElapsedGameTicks / 30f) * 0.5f) );
            light_count++;

            // Dynamic lights
            foreach(Light light in EntComponent.GetAllOfType(typeof(Light)).Cast<Light>())
            {
                light_pos[light_count] = new(light.OffsetPos.X, light.OffsetPos.Y, light.OffsetPos.Z, light.Radius);
                light_col[light_count] = light.Color;
                light_count++;
                if(light_count >= max_lights) break;
            }

            // Assemble uniforms
            vertex_uniforms.Add(new("uLightPositions", light_pos, max_lights)); 
            vertex_uniforms.Add(new("uLightColors", light_col, max_lights)); 
            vertex_uniforms.Add(new("uLightCount", light_count)); // Number of lights, not max lights
        }

        public static void RenderModel(ModelData model, List<MaterialData> materials, List<ShaderData.Uniform> vertex_uniforms)
        {
            // Render each mesh!
            int mesh_index = 0;
            foreach (var mesh in model.Meshes)
            {
                RenderMesh( mesh, materials[mesh_index], vertex_uniforms);
                mesh_index++;
            }
        }

        public static void RenderMesh(MeshData mesh, MaterialData mat_data, List<ShaderData.Uniform> vertex_uniforms)
        {
            // Check for collision drawing
            if(mesh.RawName == "col.001")
            {
                if(!Core.draw_collisions) return;
                mat_data = Core.collision_draw_material; // Use our collision shader
            }

            // Bind the VBOs
            mesh.Bind();

            // Set the blending mode
            mat_data.UseBlendMode();

            // Each mesh can use a different material, and that also means shader!
            ShaderData shader = mat_data.Shader;
            shader.Use(); 
            foreach(ShaderData.Uniform vertuni in vertex_uniforms)
            {
                shader.SetUniform(vertuni.key, vertuni.value, vertuni.count);
            }

            // Bind textures to texunits
            int tex_unit_id = 0;
            foreach(TextureData tex in mat_data.Textures)
            {
                tex.Bind((TextureUnit)tex_unit_id);
                tex_unit_id++;
            }
            
            // Apply shader uniforms
            foreach(ShaderData.Uniform matuni in mat_data.Uniforms)
            {
                shader.SetUniform(matuni.key, matuni.value, matuni.count);
            }

            // Draw mesh
            Core.OpenGLContext.DrawArrays( PrimitiveType.Triangles, 0, (uint)mesh.Indices.Length);
        }
    }
}