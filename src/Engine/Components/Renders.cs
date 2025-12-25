using Engine;

namespace EntComponents
{
    /// <summary>
    /// Component that enables the OnRender() function in an entity, and stores various other rendering information.
    /// </summary>
    public class Renders(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Visible { get; set; } = true;
        private int Priority { get; set; } = 1;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public override void RegisterSignals()
        {
            Host.RegisterSignal(Core.Signals.render, this);
        }
        public override void UnregisterSignals()
        {
            Host.UnregisterSignal(Core.Signals.render, this);
        }
        public override int ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.render_priority:
                    if(!Visible) return 0; // Do not add us to render queue
                    return Priority;

                case Core.Signals.pre_render:
                    if(Host.Enabled)
                    {
                        HandlePreRender((double)args[0]);
                    }
                    return 1;

                case Core.Signals.render:
                    if(Host.Enabled)
                    {
                        HandleRender((double)args[0]);
                    }
                    else
                    {
                        HandleRenderDisabled((double)args[0]);
                    }
                    return 1;
                    
                case Core.Signals.hud_render:
                    if(Host.Enabled)
                    {
                        HandleHudRender((double)args[0]);
                    }
                    return 1;

            }
            return 0;
        }


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Virtual functions
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual void HandlePreRender(double delta_time)
        {
            
        }

        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public virtual void HandleRender(double delta_time)
        {
            
        }

        /// <summary>
        /// Render function run if the component is NOT Visible. Mostly used for long distance LoDs.
        /// </summary>
        public virtual void HandleRenderDisabled(double delta_time)
        {
            
        }
        
        /// <summary>
        /// Render function run after all others, meant for drawing the hud over the the render canvas.
        /// </summary>
        public virtual void HandleHudRender(double delta_time)
        {
            
        }
    }
}