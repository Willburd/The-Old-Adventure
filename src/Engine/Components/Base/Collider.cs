using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that detects collisions with other colliders. Intended for handling physics.
    /// </summary>
    public class Collider(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Active { get; set; }
        private readonly bool debug_vis = false; // Debugging only
        public bool IsTrigger { get; set; } = false;

        public struct Collision(Collider source, Collider crosser)
        {
            public Collider source_collider = source;
            public Collider triggering_collider = crosser;
        }


        /// <summary>
        /// Checks against all colliders in a list and handle collisions for each.
        /// </summary>
        public void CheckCollisions(List<EntComponent> all_colliders)
        {
            // TODO - Update these from a list of colliders, to be structs of collision information, with the nearest position on the collider, the colliders involve, the origin position, distance to, etc.
            List<Collision> all_collisions = [];
            List<Collision> all_triggers = [];

            foreach(Collider col in all_colliders.Cast<Collider>())
            {
                // No self detection
                if(col == this) continue;
                // Forbid trigger reverse detection
                if(col.IsTrigger) continue;

                // Check for overlap, we must BOTH collide!
                if(IsColliding(col) && col.IsColliding(this)) 
                {
                    // Triggers only detect collisions with physics colliders, and not with other triggers
                    if(IsTrigger)
                    {
                        if(!col.IsTrigger)
                        {
                            all_triggers.Add(new Collision(this, col));
                        }
                        continue;
                    }
                    // The other colliders cannot be a trigger, so we've found an actual collision!
                    all_collisions.Add(new Collision(this, col));
                }
            }

            // Inform our host of everything we've faceplanted into this frame
            if(all_collisions.Count > 0) Host.SendSignal(Core.Signals.collision, all_collisions);
            if(all_triggers.Count > 0) Host.SendSignal(Core.Signals.trigger, all_triggers);
        }

        /// <summary>
        /// Checks if this colliders is overlapping with another collider. Returns true if so.
        /// </summary>
        public virtual bool IsColliding(Collider other_col)
        {
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> PrepareSignals()
        {
            List<Core.Signals> sig_list = [];

            if(debug_vis) 
            {
                sig_list.Add(Core.Signals.render_priority);
                sig_list.Add(Core.Signals.render);
            }

            if(IsTrigger)
            {
                sig_list.Add(Core.Signals.raycast_triggers);
            }
            else
            {
                sig_list.Add(Core.Signals.raycast_collisions);
            }

            return sig_list;
        }

        public override uint ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.render_priority:
                    // If we're not subbed, this never gets called.
                    return 256;

                case Core.Signals.render:
                    // Render our collider shape
                    return 1;
            }
            return base.ReceiveSignal(signal,args);
        }
    }
}