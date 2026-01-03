using System.Numerics;
using EntComponents;

namespace Engine
{
    public class CylinderCol(Collider host, float height, float radius) : ColShape(host)
    {
        public float height = height;
        public float radius = radius;
        
        /// <summary>
        /// Check if in range of a cylinder with a set height and radius. Origin at center.
        /// </summary>
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            Vector3 host_pos = ColHost.Position;
            Vector3 other_point = point_col.ColHost.Position;

            float radius_distance = Tools.FlattenedDistance(host_pos,other_point);
            float height_distance = MathF.Abs(host_pos.Y - other_point.Y);
            
            if(radius_distance <= radius && height_distance <= height)
            {
                return new Collider.Collision(ColHost,point_col.ColHost,other_point);
            }
            return null;
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(PlaneCol plane_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(TriCol triangle_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(WorldGeometryCol geometry_col)
        {
            return null;
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            return null;
        }
    }
}