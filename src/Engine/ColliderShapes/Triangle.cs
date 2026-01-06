using System.Numerics;
using EntComponents;
using Rendering;

namespace Engine.ColliderShapes
{
    public class TriCol(Vector3 a, Vector3 b, Vector3 c) : ColShape
    {
        public Vim.Math3d.Triangle our_triangle = new(new Vim.Math3d.Vector3(a.X,a.Y,a.Z),new Vim.Math3d.Vector3(b.X,b.Y,b.Z),new Vim.Math3d.Vector3(c.X,c.Y,c.Z));

        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            Vector3 dirvec = Tools.DirVector(ray.start_vector,ray.end_vector);
            Vim.Math3d.Ray test_ray = new( new Vim.Math3d.Vector3(ray.start_vector.X,ray.start_vector.Y,ray.start_vector.Z), new Vim.Math3d.Vector3(dirvec.X,dirvec.Y,dirvec.Z));

            float? distance = test_ray.Intersects(our_triangle);
            if(distance != null)
            {
                return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,(float)distance);
            }
            return null;
        }
        

        public override ModelData? DrawModel()
        {
            return null;
        }
    }
}