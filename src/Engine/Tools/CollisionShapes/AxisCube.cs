using System.Numerics;
using EntComponents;

namespace Engine
{
    public class AxisCubeCol(Collider host, Vector3 min, Vector3 max) : ColShape(host)
    {
        public Vim.Math3d.AABox our_box = new( new Vim.Math3d.Vector3(min.X,min.Y,min.Z), new Vim.Math3d.Vector3(max.X,max.Y,max.Z));
        
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            return SwapSourceAndHit( point_col.InOurShape(this));
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            Vector3 host_pos = ColHost.Position;
            our_box.SetCenter( new Vim.Math3d.Vector3(host_pos.X,host_pos.Y,host_pos.Z));
            
            Vector3 other_point = sphere_col.ColHost.Position;
            Vim.Math3d.Sphere other_sphere = sphere_col.our_sphere;
            other_sphere.SetCenter( new Vim.Math3d.Vector3(other_point.X,other_point.Y,other_point.Z));
            
            if(our_box.Intersects(other_sphere))
            {
                // TODO - Get collision point between cube and sphere
                //return new(ColHost,sphere_col.ColHost, );
            }
            return null;
        }

        public override Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            Vector3 host_pos = ColHost.Position;
            our_box.SetCenter( new Vim.Math3d.Vector3(host_pos.X,host_pos.Y,host_pos.Z));
            
            Vector3 other_point = box_col.ColHost.Position;
            Vim.Math3d.AABox other_box = box_col.our_box;
            other_box.SetCenter( new Vim.Math3d.Vector3(other_point.X,other_point.Y,other_point.Z));
            
            if(our_box.Intersects(other_box))
            {
                // TODO - Get collision point between cube and cube
                //return new(ColHost,sphere_col.ColHost, );
            }
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            return SwapSourceAndHit( cylinder_col.InOurShape(this));
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            Vector3 host_pos = ColHost.Position;
            our_box.SetCenter( new Vim.Math3d.Vector3(host_pos.X,host_pos.Y,host_pos.Z));
            
            // TODO - Raycast an axis aligned cube

            /*
            Vim.Math3d.Ray check_ray = new(new Vim.Math3d.Vector3(ray.start_vector.X,ray.start_vector.Y,ray.start_vector.Z),new Vim.Math3d.Vector3(ray.end_vector.X,ray.end_vector.Y,ray.end_vector.Z));
            our_box.Intersection(check_ray);

            if(distance != null) 
            {
                return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,(float)distance);
            }
            */
            return null;
        }
    }
}