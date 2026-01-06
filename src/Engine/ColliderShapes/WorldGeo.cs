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
            Vector3 dirvec = Tools.DirVector(ray.start_vector,ray.end_vector);
            Vim.Math3d.Ray test_ray = new( new Vim.Math3d.Vector3(ray.start_vector.X,ray.start_vector.Y,ray.start_vector.Z), new Vim.Math3d.Vector3(dirvec.X,dirvec.Y,dirvec.Z));

            foreach(var tri in our_mesh.CollisionTriangles)
            {
                float? distance = test_ray.Intersects(tri);
                if(distance != null)
                {
                    return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,(float)distance);
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