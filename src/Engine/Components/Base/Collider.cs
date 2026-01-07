using System.Numerics;
using Engine;
using Rendering;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that detects collisions with other colliders. Intended for handling physics.
    /// </summary>
    public class Collider(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Active { get; set; } = true;

        /// <summary>
        /// Increased with every collider spawned and periodically reset by room unloading. Used to keep track of which colliders we've been in contact with for collision start and end signals
        /// </summary>
        private static int collider_spawn_index = 0;
        private int our_collider_index = collider_spawn_index++;
        private List<int> previously_colliding_with = [];
        private List<int> in_collision_with = [];
        public List<int> GetNoLongerCollidingWith()
        {
            List<int> not_colliding_with_anymore = [];
            foreach(int ind in previously_colliding_with)
            {
                if(!in_collision_with.Contains(ind)) not_colliding_with_anymore.Add(ind);
            }
            return not_colliding_with_anymore;
        } 
        public List<int> GetStartedCollidingWith()
        {
            List<int> started_colliding_with = [];
            foreach(int ind in in_collision_with)
            {
                if(!previously_colliding_with.Contains(ind)) started_colliding_with.Add(ind);
            }
            return started_colliding_with;
        }

        /// <summary>
        /// Offset vector for the following collision transform vars
        /// </summary>
        private Vector3 col_offset;
        /// <summary>
        /// If true, the Offset getter will return a relative transformation of this vector, including rotation, from the host entity when getting the collider's Position. If false it will be the world position.
        /// </summary>
        public bool SyncRelativePosition = true;
        /// <summary>
        /// Gets the origin position of the collider. If SyncRelativePosition is true it will be tied to the host's position and rotation in world space, if false it will be a untransformed world position.
        /// </summary>
        public Vector3 OffsetPos 
        { 
            get
            {
                if(!SyncRelativePosition) return col_offset; // Use world position
                return Host.Position + Vector3.Transform(col_offset, Host.Rotation); // Use relative position, including rotation from host.
            }
            
            set
            {
                col_offset = value;
            }
        }
        

        public struct Collision
        {
            public Collision(Collider source, Collider crosser, Vector3 at_point)
            {
                source_collider = source;
                triggering_collider = crosser;
                point = at_point;
                if(!source.IsTrigger())
                {
                    all_collisions.Add(this);
                }
                else
                {
                    all_triggered.Add(this);
                }
            }
            public static List<Collision> all_collisions = [];
            public static List<Collision> all_triggered = [];
            public Collider source_collider;
            public Collider triggering_collider;
            public Vector3 point;
        } 

        public struct Raycast(Vector3 startpos, Vector3 direction)
        {
            public Vector3 start_vector = startpos;
            public Vector3 direction = direction;
        }

        public struct RaycastHit(Raycast ray, Collider hit_col, float dist)
        {
            public Vector3 start_vector = ray.start_vector;
            public Vector3 direction  = ray.direction;
            public Collider hit_collider = hit_col;
            public float distance = dist;
            public Vector3 HitPosition
            {
                get
                {
                    float perc = distance / direction.Length();
                    return Vector3.Lerp(start_vector, start_vector + direction, perc);
                }
            }
        }

        /// <summary>
        /// List of collisions previously active with US in the prior frame. Use for sending collision start and end signals.
        /// </summary>
        private List<Collider> previous_collisions = [];

        /// <summary>
        /// Performs a raycast against all existing colliders in the scene, unless otherwise specified. Returns a list of all collisions that occured, specific collision information is in each collider.
        /// </summary>
        public static List<RaycastHit> DoRaycast(Vector3 start, Vector3 direction, Entity? specific_entity = null)
        {
            List<RaycastHit> hit_rays = [];
            Raycast ray = new(start, direction);
            if(specific_entity != null)
            {
                // Specific entity check
                specific_entity.SendSignal(Core.Signals.raycast, ray, hit_rays);
            }
            else
            {
                // Global entity check
                Entity.SendGlobalSignal(Core.Signals.raycast, ray, hit_rays);
            }
            return hit_rays;
        }

        /// <summary>
        /// Each collider has a shape that is used to check against other colliders and raycasts.
        /// </summary>
        public Engine.ColliderShapes.ColShape? CollisionShape { get; private set; }
        public void SetShape( Engine.ColliderShapes.ColShape new_shape)
        {
            new_shape.ColHost = this;
            CollisionShape = new_shape;
        }

        /// <summary>
        /// Checks against all colliders in a list and handle collisions for each.
        /// </summary>
        public void CheckCollisions(List<EntComponent> all_colliders)
        {
            // Update previous collisions
            previously_colliding_with = in_collision_with;
            in_collision_with = [];

            // Keep track of current collisions
            List<Collision> all_collisions = [];
            List<Collision> all_triggers = [];

            foreach(Collider col in all_colliders.Cast<Collider>())
            {
                // No self detection
                if(col == this) continue;

                // Check for overlap
                Collision? check_collision = CheckIsColliding(col);
                if(check_collision != null) 
                {
                    // Triggers only detect collisions with physics colliders, and not with other triggers
                    if(IsTrigger())
                    {
                        if(!col.IsTrigger())
                        {
                            all_triggers.Add((Collision)check_collision);
                            in_collision_with.Add(col.our_collider_index);
                        }
                        continue;
                    }
                    // The other colliders cannot be a trigger, so we've found an actual collision!
                    all_collisions.Add((Collision)check_collision);
                    in_collision_with.Add(col.our_collider_index);
                }
            }

            // Fire a signal for all colliders we've left
            List<int> colliders_started = GetStartedCollidingWith();
            List<int> colliders_ended = GetNoLongerCollidingWith();
            foreach(Collider col in all_colliders.Cast<Collider>())
            {
                if(colliders_started.Contains(col.our_collider_index))
                {
                    if(!col.IsTrigger())
                    {
                        Host.SendSignal(Core.Signals.collision_start, col);
                    }
                    else
                    {
                        Host.SendSignal(Core.Signals.trigger_start, col);
                    }
                }
                else if(colliders_ended.Contains(col.our_collider_index))
                {
                    if(!col.IsTrigger())
                    {
                        Host.SendSignal(Core.Signals.collision_end, col);
                    }
                    else
                    {
                        Host.SendSignal(Core.Signals.trigger_end, col);
                    }
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
            return CollisionShape?.InOurShape( other_col);
        }

        /// <summary>
        /// Checks if a raycast collides with a collider.
        /// </summary>
        public uint CheckIsRayHit(Raycast ray, List<RaycastHit> hits)
        {
            if(CollisionShape == null) return 0;
            uint hit_count = 0;
            RaycastHit? hit = CollisionShape.InRay(ray);
            if(hit != null)
            {
                hits.Add((RaycastHit)hit);
                hit_count++;
            }
            return hit_count;
        }
        
        public virtual bool IsTrigger()
        {
            return false;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // Signal handling
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        public override List<Core.Signals> PrepareSignals()
        {
            List<Core.Signals> sig_list = [Core.Signals.raycast];
            if(Core.draw_collisions) 
            {
                sig_list.Add(Core.Signals.render_priority);
                sig_list.Add(Core.Signals.render);
            }
            return sig_list;
        }

        public override uint ReceiveSignal(Core.Signals signal, object[] args)
        {
            switch(signal)
            {
                case Core.Signals.render_priority:
                    // If we're not subbed, this never gets called.
                    return 255;

                case Core.Signals.render:
                    // Render our collider shape if debugging, same as above
                    return DebugRender((double)args[0]);

                case Core.Signals.raycast:
                    // Check our collision vs the incoming ray
                    return CheckIsRayHit((Raycast)args[0], (List<RaycastHit>)args[1]);
            }
            return base.ReceiveSignal(signal,args);
        }

        
        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public uint DebugRender(double tick_delta)
        {
            ModelData? model = CollisionShape?.DrawModel();
            if(model == null || CollisionShape == null) return 0;

            List<KeyValuePair<string,object>> vertex_uniforms = [];
            vertex_uniforms.Add(new("uTransform", CollisionShape.ModelTransform()));
            vertex_uniforms.Add(new("uView", Camera.GetCurrentInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uProjection", Camera.GetCurrentProjectionMatrix()));
            
            Core.RenderModel( model, [IsTrigger() ? Core.trigger_draw_material : Core.actor_collision_draw_material], vertex_uniforms);
            return 1;
        }
    }
}