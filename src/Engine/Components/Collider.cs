using Engine;

namespace EntComponents
{
    /// <summary>
    /// Component that enables the OnCollision() function in an entity, and stores various other collision information. Sends a collision signal with the two colliding objects when a collider overlap is happening.
    /// </summary>
    public class Collider(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Active { get; set; }
        private readonly bool debug_vis = false; // Debugging only

        public override void RegisterSignals()
        {
            if(debug_vis) Host.RegisterSignal(Core.Signals.render_standard, this);
        }
        public override void UnregisterSignals()
        {
            if(debug_vis) Host.UnregisterSignal(Core.Signals.render_standard, this);
        }


        /// <summary>
        /// Checks against all colliders in a list and handle collisions for each.
        /// </summary>
        public void CheckCollisions(List<EntComponent> all_colliders)
        {
            foreach(Collider col in all_colliders.Cast<Collider>())
            {
                if(col == this) continue;
                if(IsColliding(col) && col.IsColliding(this)) // we must BOTH collide!
                {
                    Host.SendSignal(Engine.Core.Signals.collision, col.Host, col);
                }
            }
        }

        /// <summary>
        /// Checks if this colliders is overlapping with another collider. Returns true if so.
        /// </summary>
        public virtual bool IsColliding(Collider other_col)
        {
            return false;
        }

        public override int ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.render_standard:
                case Core.Signals.render_on_disabled:
                    // Render our collider shape
                    return 1;
            }
            return 0;
        }
    }
}