using System.Numerics;
using EntComponents;

namespace Engine
{
    public class AxisCubeCol(Collider host, Vector3 min, Vector3 max) : ColShape(host)
    {
        public Vim.Math3d.AABox our_box = new Vim.Math3d.AABox( new Vim.Math3d.Vector3(min.X,min.Y,min.Z), new Vim.Math3d.Vector3(max.X,max.Y,max.Z));
        
        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            Vector3 host_pos = ColHost.Position;
            our_box.SetCenter( new Vim.Math3d.Vector3(host_pos.X,host_pos.Y,host_pos.Z));
            
            Vector3 other_point = point_col.ColHost.Position;
            
            if(our_box.Contains( new Vim.Math3d.Vector3(other_point.X,other_point.Y,other_point.Z)))
            {
                return new Collider.Collision(ColHost,point_col.ColHost,other_point);
            }
            return null;
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
                // TODO - Get collision point between cube and sphere
                //return new(ColHost,sphere_col.ColHost, );
            }
            return null;
        }

        public override Collider.Collision? InOurShape(CylinderCol cylinder_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(PlaneCol plane_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(TriCol triangle_col)
        {
            return null;
        }

        public override Collider.Collision? InOurShape(WorldGeometryCol geometry_col)
        {
            return null;
        }


        public override Collider.RaycastHit? InRay(Collider.Raycast ray)
        {
            return null;
        }
    }
}