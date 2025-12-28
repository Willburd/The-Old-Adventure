using Silk.NET.OpenGL;
using System.Drawing;

namespace Engine
{
    public partial class Core
    {
        public static GL OpenGLContext {get; private set;}

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
            // Default draw color
            OpenGLContext?.ClearColor(Color.CornflowerBlue);
            
            // Depth control
            OpenGLContext?.Enable(EnableCap.DepthTest);
            OpenGLContext?.DepthFunc(DepthFunction.Less);

            // Backface culling
            OpenGLContext?.Enable(EnableCap.CullFace);
            OpenGLContext?.CullFace(GLEnum.Back);

            // GLTF format
            OpenGLContext?.FrontFace(FrontFaceDirection.CW); 
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
            foreach((uint key, List<Entity> draw_list) in render_queue)
            {
                foreach(Entity draw in draw_list)
                {
                    draw.SendSignal(Signals.render, tick_delta);
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
    }
}