using System.Numerics;
using EntComponents;
using Rendering;

namespace Engine.ColliderShapes
{
    public class WorldGeometryCol(MeshData mesh) : ColShape
    {
        MeshData our_mesh = mesh;

        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            Vector3 dirvec = Tools.DirVector(Vector3.Zero,ray.direction);
            Vim.Math3d.Ray test_ray = new( new Vim.Math3d.Vector3(ray.start_vector.X,ray.start_vector.Y,ray.start_vector.Z), new Vim.Math3d.Vector3(dirvec.X,dirvec.Y,dirvec.Z));

            foreach(var tri in our_mesh.CollisionTriangles)
            {
                float? distance = test_ray.Intersects(tri);
                if(distance != null && distance <= ray.direction.Length())
                {
                    return new Collider.RaycastHit(ray, ColHost, (float)distance);
                }
            }
            return null;
        }
        

        public override ModelData? DrawModel()
        {
            return null;
        }
    }
}