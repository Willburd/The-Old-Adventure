using Engine;

namespace EntComponents
{
    /// <summary>
    /// Component that enables the OnRender() function in an entity, and stores various other rendering information.
    /// </summary>
    public class Renders(Engine.Entity host_entity) : EntComponent(host_entity)
    {
        public bool Visible { get; set; }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        public override void RegisterSignals()
        {
            Host.RegisterSignal(Core.Signals.render_standard, this);
            Host.RegisterSignal(Core.Signals.render_on_disabled, this);
        }
        public override void UnregisterSignals()
        {
            Host.UnregisterSignal(Core.Signals.render_standard, this);
            Host.UnregisterSignal(Core.Signals.render_on_disabled, this);
        }
        public override int ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.render_standard:
                    HandleRender((double)args[0]);
                    return 1;
                    
                case Core.Signals.render_on_disabled:
                    HandleRenderDisabled((double)args[0]);
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
        public virtual void HandleRender(double delta_time)
        {
            
        }

        /// <summary>
        /// Render function run if the component is NOT Visible. Mostly used for long distance LoDs.
        /// </summary>
        public virtual void HandleRenderDisabled(double delta_time)
        {
            
        }
    }
}