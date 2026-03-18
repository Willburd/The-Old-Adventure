using System.Diagnostics;
using System.Numerics;
using EntComponents;
using Assets;

namespace Engine.ColliderShapes
{
    public class WorldGeometryCol : ColShape
    {
        public const float wall_y_threshold = 0.4f; // Must match in debug_walls.frag shader

        public WorldGeometryCol(MeshData mesh)
        {
            Debug.Assert(mesh != null, "Collision mesh was assigned as null");
            our_mesh = mesh;
        }
        MeshData our_mesh;

        public int MeshTriCount()
        {
            return our_mesh.CollisionTriangles.Count;
        }

        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            Vector3 normal_ray = Vector3.Normalize(ray.direction);
            Vim.Math3d.Ray check_ray = new(new Vim.Math3d.Vector3(ray.start_vector.X - ColHost.OffsetPos.X, ray.start_vector.Y - ColHost.OffsetPos.Y, ray.start_vector.Z - ColHost.OffsetPos.Z), new Vim.Math3d.Vector3(normal_ray.X, normal_ray.Y, normal_ray.Z));

            foreach (var tri in our_mesh.CollisionTriangles)
            {
                float? distance = check_ray.Intersects(tri);
                Vector3 normal = new Vector3(tri.Normal.X, tri.Normal.Y, tri.Normal.Z);
                if (distance != null && distance <= ray.direction.Length() && Vector3.Dot(ray.direction, normal) < 0)
                {
                    return new Collider.RaycastHit(ray, ColHost, (float)distance, normal, tri);
                }
            }
            return null;
        }

        public override MeshData? DrawModel()
        {
            return our_mesh;
        }
    }
}