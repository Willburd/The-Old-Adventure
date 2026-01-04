using System.Numerics;
using EntComponents;

namespace Engine
{
    public class PointCol(Collider host) : ColShape(host)
    {
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            Vector3 host_pos = ColHost.Position;
            host_pos.X = MathF.Round(host_pos.X);
            host_pos.Y = MathF.Round(host_pos.Y);
            host_pos.Z = MathF.Round(host_pos.Z);

            Vector3 other_point = point_col.ColHost.Position + Vector3.Transform(point_col.ColHost.Position,point_col.ColHost.Host.Rotation);
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
            Vector3 other_point = sphere_col.ColHost.Position;
            sphere_col.our_sphere.SetCenter( new Vim.Math3d.Vector3(other_point.X,other_point.Y,other_point.Z));
            
            Vim.Math3d.ContainmentType contype = sphere_col.our_sphere.Contains( new Vim.Math3d.Vector3(ColHost.Position.X,ColHost.Position.Y,ColHost.Position.Z));
            if(contype == Vim.Math3d.ContainmentType.Contains || contype == Vim.Math3d.ContainmentType.Intersects)
            {
                return new Collider.Collision(ColHost,sphere_col.ColHost,ColHost.Position);
            }
            return null;
        }

        public override Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            Vector3 other_point = box_col.ColHost.Position;
            box_col.our_box.SetCenter( new Vim.Math3d.Vector3(other_point.X,other_point.Y,other_point.Z));
            
            if(box_col.our_box.Contains( new Vim.Math3d.Vector3(ColHost.Position.X,ColHost.Position.Y,ColHost.Position.Z)))
            {
                return new Collider.Collision(ColHost,box_col.ColHost,ColHost.Position);
            }
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            Vector3 other_point = cylinder_col.ColHost.Position;

            float radius_distance = Tools.FlattenedDistance(ColHost.Position,other_point);
            float height_distance = MathF.Abs(ColHost.Position.Y - other_point.Y);
            
            if(radius_distance <= cylinder_col.radius && height_distance <= cylinder_col.height)
            {
                return new Collider.Collision(ColHost,cylinder_col.ColHost,ColHost.Position);
            }
            return null;
            
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            float our_dist = Vector3.Distance(ray.start_vector, ColHost.Position);
            float dot_product = Vector3.Dot( Tools.DirVector(ray.start_vector,ray.end_vector), Tools.DirVector(ray.start_vector,ColHost.Position));

            if(dot_product == 1f && our_dist <= Vector3.Distance(ray.start_vector,ray.end_vector))
            {
                return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,our_dist);
            }
            return null;
        }
    }
}