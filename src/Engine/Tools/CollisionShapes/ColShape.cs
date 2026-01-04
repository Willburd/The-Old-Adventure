using System.Numerics;
using EntComponents;

namespace Engine
{
    public class ColShape(Collider host)
    {
        public Collider ColHost { get; private set;} = host;

        public Collider.Collision? InOurShape(Collider other_collider)
        {
            if(other_collider.CollisionShape == null) return null;

            Type collision_type = other_collider.CollisionShape.GetType();
            if(collision_type == typeof(PointCol))          return InOurShape( (PointCol)other_collider.CollisionShape);
            if(collision_type == typeof(SphereCol))         return InOurShape( (SphereCol)other_collider.CollisionShape);
            if(collision_type == typeof(AxisCubeCol))       return InOurShape( (AxisCubeCol)other_collider.CollisionShape);
            if(collision_type == typeof(CylinderCol))       return InOurShape( (CylinderCol)other_collider.CollisionShape);

            return null;
        }

        protected Collider.Collision? SwapSourceAndHit(Collider.Collision? col_hit)
        {
            if(col_hit.HasValue) // Swap source and trigger
            {
                Collider.Collision acol = col_hit.Value;

                Collider temp = acol.source_collider;
                acol.source_collider = ColHost;
                acol.triggering_collider = temp;
                
                return acol;
            }
            return col_hit;
        }

        public virtual Collider.Collision? InOurShape(PointCol point_col)
        {
            return null;
        }

        public virtual Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            return null;
        }

        public virtual Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            return null;
        }

        public virtual Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            return null;
        }

        /// <summary>
        /// Check if in raycast intersects the collision shape.
        /// </summary>
        public virtual Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            return null;
        }
    }
}