using System.Numerics;
using EntComponents;
using Rendering;
using Engine;

namespace ColliderShapes
{
    public class SphereCol(float radius) : ColShape
    {
        public Vim.Math3d.Sphere our_sphere = new(Vim.Math3d.Vector3.Zero, radius);

        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            return SwapSourceAndHit( point_col.InOurShape(this));
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            Vector3 host_pos = ColHost.Position;
            our_sphere.SetCenter( new Vim.Math3d.Vector3(host_pos.X,host_pos.Y,host_pos.Z));
            
            Vector3 other_point = sphere_col.ColHost.Position;
            Vim.Math3d.Sphere other_sphere = sphere_col.our_sphere;
            other_sphere.SetCenter( new Vim.Math3d.Vector3(other_point.X,other_point.Y,other_point.Z));
            
            if(our_sphere.Intersects(other_sphere))
            {
                Vector3 col_vector = ColHost.Position + (Tools.DirVector(ColHost.Position, sphere_col.ColHost.Position) * our_sphere.Radius); // from us to them
                Vector3 ret_vector = sphere_col.ColHost.Position + (Tools.DirVector(sphere_col.ColHost.Position, ColHost.Position) * other_sphere.Radius); // from them to us
                return new(ColHost,sphere_col.ColHost, Vector3.Lerp(col_vector,ret_vector,0.5f)); // Get a point between!
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
            our_sphere.SetCenter( new Vim.Math3d.Vector3(host_pos.X,host_pos.Y,host_pos.Z));
            
            Vim.Math3d.Ray check_ray = new(new Vim.Math3d.Vector3(ray.start_vector.X,ray.start_vector.Y,ray.start_vector.Z),new Vim.Math3d.Vector3(ray.end_vector.X,ray.end_vector.Y,ray.end_vector.Z));
            float? distance = our_sphere.Intersects(check_ray);

            if(distance != null) 
            {
                return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,(float)distance);
            }
            return null;
        }
        

        public override ModelData? DrawModel()
        {
            return Core.collision_model_sphere;
        }

        public override Matrix4x4 ModelTransform()
        {
            return Matrix4x4.Identity * Matrix4x4.CreateScale(new Vector3(our_sphere.Radius, our_sphere.Radius, our_sphere.Radius)) * Matrix4x4.CreateTranslation(ColHost.Position);
        }
    }
}