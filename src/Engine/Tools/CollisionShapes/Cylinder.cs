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
            
            Vector3 host_pos = ColHost.Position;
            float our_rad = radius;
            float our_top = host_pos.Y + (height*0.5f);
            float our_bottom = host_pos.Y - (height*0.5f);

            Vector3 other_pos = cylinder_col.ColHost.Position;
            float other_rad = cylinder_col.radius;
            float other_top = other_pos.Y + (cylinder_col.height*0.5f);
            float other_bottom = other_pos.Y - (cylinder_col.height*0.5f);

            if(other_top >= our_bottom || other_bottom <= our_top)
            {
                if(Tools.FlattenedDistance(host_pos, other_pos) <= our_rad + other_rad)
                {
                    // Horizontal midpoint
                    Vector3 col_vector = ColHost.Position + (Tools.DirVector(ColHost.Position, cylinder_col.ColHost.Position) * radius); // from us to them
                    Vector3 ret_vector = cylinder_col.ColHost.Position + (Tools.DirVector(cylinder_col.ColHost.Position, ColHost.Position) * cylinder_col.radius); // from them to us
                    Vector3 mid_pos = Vector3.Lerp(col_vector,ret_vector,0.5f); // Get a point between!

                    // Vertical midpoint
                    float mid_y = float.Lerp(ColHost.Position.Y,cylinder_col.ColHost.Position.Y, 0.5f);
                    mid_y = MathF.Min(mid_y, our_top);
                    mid_y = MathF.Min(mid_y, other_top);
                    mid_y = MathF.Max(mid_y, our_bottom);
                    mid_y = MathF.Max(mid_y, other_bottom);
                    mid_pos.Y = mid_y;

                    return new(ColHost, cylinder_col.ColHost, mid_pos);
                }
            }
            return null;
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            return null;
        }
    }
}