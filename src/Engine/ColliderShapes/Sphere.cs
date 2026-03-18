using System.Numerics;
using EntComponents;
using Assets;

namespace Engine.ColliderShapes
{
    public class SphereCol(float radius) : ColShape
    {
        public Vim.Math3d.Sphere our_sphere = new(Vim.Math3d.Vector3.Zero, radius);

        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            return SwapSourceAndHit(point_col.InOurShape(this));
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            Vector3 host_pos = ColHost.OffsetPos;
            Vector3 other_point = sphere_col.ColHost.OffsetPos;

            if (Vector3.Distance(host_pos, other_point) <= our_sphere.Radius + sphere_col.our_sphere.Radius)
            {
                Vector3 col_vector = ColHost.OffsetPos + (Tools.DirVector(sphere_col.ColHost.OffsetPos, ColHost.OffsetPos) * our_sphere.Radius); // from us to them
                Vector3 ret_vector = sphere_col.ColHost.OffsetPos + (Tools.DirVector(ColHost.OffsetPos, sphere_col.ColHost.OffsetPos) * sphere_col.our_sphere.Radius); // from them to us
                return new(ColHost, sphere_col.ColHost, Vector3.Lerp(col_vector, ret_vector, 0.5f)); // Get a point between!
            }
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            return SwapSourceAndHit(cylinder_col.InOurShape(this));
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            // We check the ray offset, as moving the sphere's center did not work for raycasting
            Vector3 normal_ray = Vector3.Normalize(ray.direction);
            Vim.Math3d.Ray check_ray = new(new Vim.Math3d.Vector3(ray.start_vector.X - ColHost.OffsetPos.X, ray.start_vector.Y - ColHost.OffsetPos.Y, ray.start_vector.Z - ColHost.OffsetPos.Z), new Vim.Math3d.Vector3(normal_ray.X, normal_ray.Y, normal_ray.Z));
            float? distance = our_sphere.Intersects(check_ray);

            if (distance != null && distance <= ray.direction.Length())
            {
                return new Collider.RaycastHit(ray, ColHost, (float)distance, Tools.DirVector(ColHost.OffsetPos, ray.start_vector + (Vector3.Normalize(ray.direction) * (float)distance)), null);
            }
            return null;
        }


        public override MeshData? DrawModel()
        {
            return Core.collision_model_sphere.Meshes[0];
        }

        public override Matrix4x4 ModelTransform()
        {
            return Matrix4x4.Identity * Matrix4x4.CreateScale(new Vector3(our_sphere.Radius, our_sphere.Radius, our_sphere.Radius)) * Matrix4x4.CreateTranslation(ColHost.OffsetPos);
        }
    }
}