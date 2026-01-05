using System.Numerics;
using EntComponents;

namespace Engine
{
    public class CylinderCol(Collider host, float height, float radius) : ColShape(host)
    {
        public float height = height;
        public float radius = radius;

        private bool InRadius(Vector3 center, Vector3 check_point, float check_radius)
        {
            return Tools.FlattenedDistance(center, check_point) <= check_radius;
        }
        
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            return SwapSourceAndHit( point_col.InOurShape(this));
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            Console.WriteLine("=========================== TODO - cylinder collider to sphere collision");
            return null;
        }

        public override Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            Vector3 other_pos = box_col.ColHost.Position;
            box_col.our_box.SetCenter( new Vim.Math3d.Vector3(other_pos.X,other_pos.Y,other_pos.Z));

            foreach(Vim.Math3d.Vector3 vec in box_col.our_box.Corners)
            {
                Vector3 corner = new Vector3(vec.X,vec.Y,vec.Z);
                if(InRadius(ColHost.Position,corner,radius))
                {
                    Vector3 rad_vector = ColHost.Position + (Tools.DirVector(ColHost.Position, corner) * radius); // from us to the point by our radius
                    Vector3 dist_vector = ColHost.Position + (Tools.DirVector(ColHost.Position, corner) * Vector3.Distance(ColHost.Position, corner));
                    Vector3 mid_pos = Vector3.Lerp(rad_vector,dist_vector,0.5f); // Get a point between!

                    return new(ColHost, box_col.ColHost, mid_pos);
                }
            }
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            
            Vector3 host_pos = ColHost.Position;
            float our_rad = radius;
            float our_top = host_pos.Y + height;
            float our_bottom = host_pos.Y;

            Vector3 other_pos = cylinder_col.ColHost.Position;
            float other_rad = cylinder_col.radius;
            float other_top = other_pos.Y + cylinder_col.height;
            float other_bottom = other_pos.Y;

            if(other_top >= our_bottom || other_bottom <= our_top)
            {
                if(InRadius(host_pos, other_pos, our_rad + other_rad))
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