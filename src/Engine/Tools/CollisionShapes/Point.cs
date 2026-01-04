using System.Numerics;
using EntComponents;

namespace Engine
{
    public class PointCol(Collider host, Vector3 offset) : ColShape(host)
    {
        public Vector3 our_point = new(offset.X,offset.Y,offset.Z);

        
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            Vector3 host_pos = ColHost.Position + Vector3.Transform(our_point,ColHost.Host.Rotation);
            host_pos.X = MathF.Round(host_pos.X);
            host_pos.Y = MathF.Round(host_pos.Y);
            host_pos.Z = MathF.Round(host_pos.Z);

            Vector3 other_point = point_col.ColHost.Position + Vector3.Transform(point_col.our_point,point_col.ColHost.Host.Rotation);
            other_point.X = MathF.Round(other_point.X);
            other_point.Y = MathF.Round(other_point.Y);
            other_point.Z = MathF.Round(other_point.Z);

            if(host_pos == other_point)
            {
                return new(ColHost,point_col.ColHost, host_pos);
            }
            return null;
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            return SwapSourceAndHit( sphere_col.InOurShape(this));
        }

        public override Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            return SwapSourceAndHit( box_col.InOurShape(this));
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            return SwapSourceAndHit( cylinder_col.InOurShape(this));
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            float our_dist = Vector3.Distance(ray.start_vector, our_point);
            float dot_product = Vector3.Dot( Tools.DirVector(ray.start_vector,ray.end_vector), Tools.DirVector(ray.start_vector,our_point));

            if(dot_product == 1f && our_dist <= Vector3.Distance(ray.start_vector,ray.end_vector))
            {
                return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,our_dist);
            }
            return null;
        }
    }
}