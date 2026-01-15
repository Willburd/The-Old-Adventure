using System.Numerics;
using EntComponents;
using Assets;

namespace Engine.ColliderShapes
{
    public class TriCol(Vector3 a, Vector3 b, Vector3 c) : ColShape
    {
        public Vim.Math3d.Triangle our_triangle = new(new Vim.Math3d.Vector3(a.X, a.Y, a.Z), new Vim.Math3d.Vector3(b.X, b.Y, b.Z), new Vim.Math3d.Vector3(c.X, c.Y, c.Z));

        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            Vector3 normal_ray = Vector3.Normalize(ray.direction);
            Vim.Math3d.Ray test_ray = new(new Vim.Math3d.Vector3(ray.start_vector.X, ray.start_vector.Y, ray.start_vector.Z), new Vim.Math3d.Vector3(normal_ray.X, normal_ray.Y, normal_ray.Z));

            float? distance = test_ray.Intersects(our_triangle);
            if (distance != null && distance <= ray.direction.Length())
            {
                return new Collider.RaycastHit(ray, ColHost, (float)distance, new Vector3(our_triangle.Normal.X, our_triangle.Normal.Y, our_triangle.Normal.Z), our_triangle);
            }
            return null;
        }

        public override MeshData? DrawModel()
        {
            return null;
        }
    }
}