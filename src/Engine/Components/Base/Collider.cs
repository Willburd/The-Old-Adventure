using System.Numerics;
using Engine;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that detects collisions with other colliders. Intended for handling physics.
    /// </summary>
    public class Collider(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Active { get; set; }
        public bool IsTrigger { get; set; } = false;
        
        private readonly bool debug_vis = false; // Debugging only

        public struct Collision(Collider source, Collider crosser)
        {
            public Collider source_collider = source;
            public Collider triggering_collider = crosser;
        }

        public struct Raycast(Vector3 startpos, Vector3 endpos, ColliderType checking_for)
        {
            public Vector3 start_vector = startpos;
            public Vector3 end_vector = endpos;
            public ColliderType check_type = checking_for;
        }

        public struct RaycastHit(Vector3 startpos, Vector3 endpos, Collider hit_col, Vector3 hit_pos)
        {
            public Vector3 start_vector = startpos;
            public Vector3 end_vector  = endpos;
            public Collider hit_collider = hit_col;
            public Vector3 hit_position = hit_pos;
        }

        public static List<RaycastHit> DoRaycast(Vector3 start, Vector3 end, ColliderType filter)
        {
            List<RaycastHit> hit_rays = [];
            Raycast ray = new(start, end, filter);
            Entity.SendGlobalSignal(Core.Signals.global_raycast, ray, hit_rays);

            return hit_rays;
        }

        /// <summary>
        /// Each collider has a shape that is used to check against other colliders and raycasts.
        /// </summary>
        protected ColliderType CollisionShape { get; set; } = ColliderType.None;
        public enum ColliderType
        {
            None,
            Plane,
            AxisBox,
            VertRadius,
            Sphere,
            Mesh
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

                // Check for overlap
                Collision? check_collision = CheckIsColliding(col);
                if(check_collision != null) 
                {
                    // Triggers only detect collisions with physics colliders, and not with other triggers
                    if(IsTrigger)
                    {
                        if(!col.IsTrigger)
                        {
                            all_triggers.Add((Collision)check_collision);
                        }
                        continue;
                    }
                    // The other colliders cannot be a trigger, so we've found an actual collision!
                    all_collisions.Add((Collision)check_collision);
                }
            }

            // Inform our host of everything we've faceplanted into this frame
            if(all_collisions.Count > 0) Host.SendSignal(Core.Signals.collision, all_collisions);
            if(all_triggers.Count > 0) Host.SendSignal(Core.Signals.trigger, all_triggers);
        }

        /// <summary>
        /// Checks if this collider is overlapping with another collider.
        /// </summary>
        public Collision? CheckIsColliding(Collider other_col)
        {
            return null;
        }

        /// <summary>
        /// Checks if a raycast collides with a collider.
        /// </summary>
        public uint CheckIsRayHit(Raycast ray, List<RaycastHit> hits)
        {
            return 0;
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
            sig_list.Add(Core.Signals.global_raycast);
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

                case Core.Signals.global_raycast:
                    // Check our collision vs the incoming ray
                    return CheckIsRayHit((Raycast)args[0], (List<RaycastHit>)args[1]);
            }
            return base.ReceiveSignal(signal,args);
        }
    }
}