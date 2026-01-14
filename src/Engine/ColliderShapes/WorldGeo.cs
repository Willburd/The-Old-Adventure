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

        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            Vim.Math3d.Ray check_ray = new(new Vim.Math3d.Vector3(ray.start_vector.X - ColHost.OffsetPos.X, ray.start_vector.Y - ColHost.OffsetPos.Y, ray.start_vector.Z - ColHost.OffsetPos.Z), new Vim.Math3d.Vector3(ray.direction.X, ray.direction.Y, ray.direction.Z));

            foreach (var tri in our_mesh.CollisionTriangles)
            {
                float? distance = check_ray.Intersects(tri);
                if (distance != null && distance <= ray.direction.Length())
                {
                    return new Collider.RaycastHit(ray, ColHost, (float)distance, new Vector3(tri.Normal.X, tri.Normal.Y, tri.Normal.Z));
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