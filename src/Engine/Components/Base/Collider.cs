using System.Numerics;
using Engine;
using Assets;

namespace EntComponents
{
    /// <summary>
    /// Entity Component that detects collisions with other colliders. Intended for handling physics.
    /// </summary>
    public class Collider(Entity host_entity) : EntComponent(host_entity)
    {
        public bool Active { get; set; } = true;

        /// <summary>
        /// Checked against a bitmask when doing collision detection. Allows for raycasts and other colliders to entirely ignore other colliders if masked correctly.
        /// </summary>
        public uint CollisionMask { get; set; } = mask_standard;
        public const uint mask_standard = 1 << 0;

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

        public struct Raycast(Vector3 startpos, Vector3 direction, uint collision_mask)
        {
            public Vector3 start_vector = startpos;
            public Vector3 direction = direction;
            public uint collision_mask = collision_mask;
        }

        public struct RaycastHit(Raycast ray, Collider hit_col, float dist)
        {
            public Vector3 StartPos { get; set; } = ray.start_vector;
            public Vector3 direction  = ray.direction;
            public Collider hit_collider = hit_col;
            public float distance = dist;
            public Vector3 HitPosition
            {
                get
                {
                    float perc = distance / direction.Length();
                    return Vector3.Lerp(StartPos, StartPos + direction, perc);
                }
            }
        }

        /// <summary>
        /// Performs a raycast against all existing colliders in the room, unless otherwise specified. Returns a list of all collisions that occured, specific collision information is in each collider.
        /// </summary>
        public static RaycastHit? DoRaycastNearest(Vector3 start, Vector3 direction, uint collision_mask = mask_standard, Entity? specific_entity = null)
        {
            List<RaycastHit> hits = DoRaycast(start, direction, collision_mask, specific_entity);
            
            float dist = float.PositiveInfinity;
            RaycastHit? nearest_hit = null;
            foreach(RaycastHit hit in hits)
            {
                if(hit.distance < dist)
                {
                    nearest_hit = hit;
                    dist = hit.distance;
                }
            }
            return nearest_hit;
        }

        /// <summary>
        /// Performs a raycast against all existing colliders in the room, unless otherwise specified. Returns a list of all collisions that occured, specific collision information is in each collider.
        /// </summary>
        public static List<RaycastHit> DoRaycast(Vector3 start, Vector3 direction, uint collision_mask = mask_standard, Entity? specific_entity = null)
        {
            List<RaycastHit> hit_rays = [];
            Raycast ray = new(start, direction, collision_mask);
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
                if((col.CollisionMask & CollisionMask) == 0) continue; 

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
            if((ray.collision_mask & CollisionMask) == 0) return 0;

            RaycastHit? hit = CollisionShape.InRay(ray);
            if(hit == null) return 0;
            hits.Add((RaycastHit)hit);
            return 1;
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
                    return DebugRender((double)args[0], (List<ShaderData.Uniform>)args[1]);

                case Core.Signals.raycast:
                    // Check our collision vs the incoming ray
                    return CheckIsRayHit((Raycast)args[0], (List<RaycastHit>)args[1]);
            }
            return base.ReceiveSignal(signal,args);
        }

        /// <summary>
        /// Render function run if the component is Visible.
        /// </summary>
        public uint DebugRender(double tick_delta, List<ShaderData.Uniform> vertex_uniforms)
        {
            MeshData? model = CollisionShape?.DrawModel();
            if(model == null || CollisionShape == null) return 0;

            // position uniforms
            vertex_uniforms.Add(new("uTransform", CollisionShape.ModelTransform()));
            vertex_uniforms.Add(new("uView", Camera.GetCurrentInterpolatedViewMatrix(tick_delta)));
            vertex_uniforms.Add(new("uProjection", Camera.GetCurrentProjectionMatrix()));

            Core.RenderMesh( model, IsTrigger() ? Core.trigger_draw_material : Core.actor_collision_draw_material, vertex_uniforms);
            return 1;
        }
    }
}