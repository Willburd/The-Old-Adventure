using System.Numerics;
using EntComponents;
using Rendering;

namespace Engine.ColliderShapes
{
    public class PointCol : ColShape
    {
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            Vector3 host_pos = ColHost.OffsetPos;
            host_pos.X = MathF.Round(host_pos.X);
            host_pos.Y = MathF.Round(host_pos.Y);
            host_pos.Z = MathF.Round(host_pos.Z);

            Vector3 other_point = point_col.ColHost.OffsetPos + Vector3.Transform(point_col.ColHost.OffsetPos,point_col.ColHost.Host.Rotation);
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
            Vector3 other_point = sphere_col.ColHost.OffsetPos;

            if(Vector3.Distance(ColHost.OffsetPos,other_point) <= sphere_col.our_sphere.Radius)
            {
                return new Collider.Collision(ColHost,sphere_col.ColHost,ColHost.OffsetPos);
            }
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            Vector3 other_point = cylinder_col.ColHost.OffsetPos;

            float radius_distance = Tools.FlattenedDistance(ColHost.OffsetPos,other_point);
            float height_distance = MathF.Abs(ColHost.OffsetPos.Y - (other_point.Y + (cylinder_col.height/2f)));
            
            if(radius_distance <= cylinder_col.radius && height_distance <= (cylinder_col.height / 2f))
            {
                return new Collider.Collision(ColHost,cylinder_col.ColHost,ColHost.OffsetPos);
            }
            return null;
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            float our_dist = Vector3.Distance(ray.start_vector, ColHost.OffsetPos);
            float dot_product = Vector3.Dot( Tools.DirVector(ray.start_vector,ray.end_vector), Tools.DirVector(ray.start_vector,ColHost.OffsetPos));

            if(dot_product == 1f && our_dist <= Vector3.Distance(ray.start_vector,ray.end_vector))
            {
                return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,our_dist);
            }
            return null;
        }
        

        public override ModelData? DrawModel()
        {
            return Core.collision_model_point;
        }

        public override Matrix4x4 ModelTransform()
        {
            return Matrix4x4.Identity * Matrix4x4.CreateScale(new Vector3(0.02f,0.02f,0.02f)) * Matrix4x4.CreateTranslation(ColHost.OffsetPos);
        }
    }
}