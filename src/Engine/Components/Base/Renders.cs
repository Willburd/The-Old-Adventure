using Engine;
using Silk.NET.OpenGL;

namespace EntComponents
{
    public class Renders(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Visible { get; set; } = true;
        private uint Priority { get; set; } = 1;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public override List<Core.Signals> DefaultSignals()
        {
            return [Core.Signals.asset_load, Core.Signals.create, Core.Signals.render_priority, Core.Signals.render, Core.Signals.hud_render];
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
        public virtual uint HandleRender(double delta_time)
        {
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