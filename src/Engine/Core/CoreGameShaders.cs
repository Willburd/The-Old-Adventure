using Assets;
using System.Drawing;
using System.Numerics;
using EntComponents;
using Rendering;

namespace Engine
{
    public partial class Core
    {
        public const int max_lights = 16; // Must match in shader
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
        /// Create framebuffers for complex rendering tasks and down/upsampling.
        /// </summary>
        public virtual void CreateFrameBuffers()
        {
            uint render_size = InternalRenderResolutionHeight;
            if (InternalRenderScale > 0f) render_size = (uint)(DisplayHeight * InternalRenderScale);
            FrameBuffer_Game = new Rendering.FrameBufferContainer(GetAspectWidth(render_size), render_size);
        }

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

        /// <summary>
        /// Assemble vertex uniforms in a proper format for shaders
        /// </summary>
        private static void ApplyVertexUniforms(List<ShaderData.Uniform> vertex_uniforms, Vector4[] light_pos_array, Vector4[] light_color_array, int light_count, Vector4 fog_color, float fog_distance)
        {
            vertex_uniforms.Add(new("uLightPositions", light_pos_array));
            vertex_uniforms.Add(new("uLightColors", light_color_array));
            vertex_uniforms.Add(new("uLightCount", light_count)); // Number of lights, not max lights
            vertex_uniforms.Add(new("uFogColor", fog_color));
            vertex_uniforms.Add(new("uFogDistance", fog_distance));
        }
    }
}