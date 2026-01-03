using System.Numerics;
using EntComponents;

namespace Engine
{
    public class PointCol(Collider host, Vector3 offset) : ColShape(host)
    {
        public Vim.Math3d.Vector3 our_point = new(offset.X,offset.Y,offset.Z);

        
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            Vim.Math3d.Vector3 rounded_point = our_point;
            rounded_point.SetX(MathF.Round(rounded_point.X));
            rounded_point.SetY(MathF.Round(rounded_point.Y));
            rounded_point.SetZ(MathF.Round(rounded_point.Z));

            Vim.Math3d.Vector3 other_rounded_point = point_col.our_point;
            other_rounded_point.SetX(MathF.Round(other_rounded_point.X));
            other_rounded_point.SetY(MathF.Round(other_rounded_point.Y));
            other_rounded_point.SetZ(MathF.Round(other_rounded_point.Z));

            if(rounded_point == other_rounded_point)
            {
                return new(ColHost,point_col.ColHost, new Vector3(rounded_point.X,rounded_point.Y,rounded_point.Z));
            }
            return null;
        }

        public override Collider.Collision? InOurShape(SphereCol sphere_col)
        {
            Collider.Collision? col_hit = sphere_col.InOurShape(this);
            if(col_hit.HasValue) // Swap source and trigger
            {
                Collider.Collision acol = col_hit.Value;
                acol.source_collider = ColHost;
                acol.triggering_collider = sphere_col.ColHost;
            }
            return col_hit;
        }

        public override Collider.Collision? InOurShape(AxisCubeCol box_col)
        {
            Collider.Collision? col_hit = box_col.InOurShape(this);
            if(col_hit.HasValue) // Swap source and trigger
            {
                Collider.Collision acol = col_hit.Value;
                acol.source_collider = ColHost;
                acol.triggering_collider = box_col.ColHost;
            }
            return col_hit;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            Collider.Collision? col_hit = cylinder_col.InOurShape(this);
            if(col_hit.HasValue) // Swap source and trigger
            {
                Collider.Collision acol = col_hit.Value;
                acol.source_collider = ColHost;
                acol.triggering_collider = cylinder_col.ColHost;
            }
            return col_hit;
        }

        public override Collider.Collision? InOurShape(PlaneCol plane_col)
        {
            // Cannot collide with
            return null;
        }

        public override Collider.Collision? InOurShape(TriCol triangle_col)
        {
            // Cannot collide with
            return null;
        }

        public override Collider.Collision? InOurShape(WorldGeometryCol geometry_col)
        {
            // Cannot collide with
            return null;
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            float our_dist = Vector3.Distance(ray.start_vector, new Vector3(our_point.X,our_point.Y,our_point.Z));
            float dot_product = Vector3.Dot( Tools.DirVector(ray.start_vector,ray.end_vector), Tools.DirVector(ray.start_vector,new Vector3(our_point.X,our_point.Y,our_point.Z)));

            if(dot_product == 1f && our_dist <= Vector3.Distance(ray.start_vector,ray.end_vector))
            {
                return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,our_dist);
            }
            return null;
        }
    }
}