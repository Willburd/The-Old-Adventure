
namespace Engine
{
    public partial class Core
    {
        /// <summary>
        /// Virtual function for game specific behaviors. Called at the start of the Engine.Core's constructor, before anything else happens.
        /// </summary>
        public virtual void OnPreInit() { }

        /// <summary>
        /// Virtual function for adventure specific behaviors. Used to load adventure specific assets during engine init.
        /// </summary>
        public virtual void OnLoadAssets() { }

        /// <summary>
        /// Virtual function for adventure specific behaviors. Called after the game window has be started, after all engine setup has completed.
        /// </summary>
        public virtual void OnInit() { }

        /// <summary>
        /// Virtual function for adventure specific gamestart code. Called when a file is created, as is expected to set the default starting savestate of the adventure.
        /// </summary>
        public virtual void OnNewGame() { }

        /// <summary>
        /// Virtual function for adventure specific behaviors. Called at the start of MainLoopEnd(), to allow cleanup of adventure specific engine code.
        /// </summary>
        public virtual void OnEnd() { }

        /// <summary>
        /// Virtual function for game specific behaviors. Called during the start of a game tick, before anything is processed.
        /// </summary>
        public virtual void OnPreGameTick() { }

        /// <summary>
        /// Virtual function for game specific behaviors. Called before the game tick processes all active entities.
        /// </summary>
        public virtual void OnGameTick() { }

        /// <summary>
        /// Virtual function that called before the game tick processes all active physics components.
        /// </summary>
        public virtual void OnPhysicsTick() { }

        /// <summary>
        /// Virtual function for game specific behaviors. Called after the game tick processes all active entities.
        /// </summary>
        public virtual void OnPostGameTick() { }

        /// <summary>
        /// Virtual function for game specific behaviors. Called during the start of a render tick, before anything is rendered.
        /// </summary>
        public virtual void OnPreRenderTick() { }

        /// <summary>
        /// Virtual function for game specific behaviors. Called at the start of a render tick, before everything is renderer
        /// </summary>
        public virtual void OnRenderTick() { }

        /// <summary>
        /// Virtual function for game specific behaviors. Called after the standard render tick, before the hud is rendered.
        /// </summary>
        public virtual void OnPostRenderTick() { }

        /// <summary>
        /// Virtual function for game specific behaviors. Called at the end of a render tick, before the hud is rendered.
        /// </summary>
        public virtual void OnRenderHudTick() { }
    }
}