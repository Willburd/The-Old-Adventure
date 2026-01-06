using System.Numerics;
using EntComponents;
using Rendering;
using Engine;

namespace ColliderShapes
{
    public class AxisCubeCol(Vector3 min, Vector3 max) : ColShape
    {
        public Vim.Math3d.AABox our_box = new( new Vim.Math3d.Vector3(min.X,min.Y,min.Z), new Vim.Math3d.Vector3(max.X,max.Y,max.Z));
        
        private Vector3 Size()
        {
            Vim.Math3d.Vector3 siz = our_box.Max - our_box.Min;
            return new Vector3(siz.X,siz.Y,siz.Z);
        }

        public override Collider.Collision? InOurShape(PointCol point_col)
        {
            return SwapSourceAndHit( point_col.InOurShape(this));
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
                Vector3 dist_vector = ColHost.Position + (Tools.DirVector(ColHost.Position, sphere_col.ColHost.Position) * Size()); // from us to them, by our size
                Vector3 rad_vector = sphere_col.ColHost.Position + (Tools.DirVector(sphere_col.ColHost.Position, ColHost.Position) * other_sphere.Radius); // from them to us with their radius
                Vector3 mid_pos = Vector3.Lerp(dist_vector,rad_vector,0.5f); // Get a point between!
                
                return new(ColHost,sphere_col.ColHost, mid_pos);
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
                Vector3 ouredge_vector = ColHost.Position + (Tools.DirVector(ColHost.Position, box_col.ColHost.Position) * Size()); // from us to them, by our size
                Vector3 theiredge_vector = box_col.ColHost.Position + (Tools.DirVector(box_col.ColHost.Position, ColHost.Position) * box_col.Size()); // from them to us, by their size
                Vector3 mid_pos = Vector3.Lerp(ouredge_vector,theiredge_vector,0.5f); // Get a point between!
                
                return new(ColHost,box_col.ColHost, mid_pos);
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
            our_box.SetCenter( new Vim.Math3d.Vector3(host_pos.X,host_pos.Y,host_pos.Z));
            
            Console.WriteLine("=========================== TODO - Raycast an axis aligned cube");

            /*
            Vim.Math3d.Ray check_ray = new(new Vim.Math3d.Vector3(ray.start_vector.X,ray.start_vector.Y,ray.start_vector.Z),new Vim.Math3d.Vector3(ray.end_vector.X,ray.end_vector.Y,ray.end_vector.Z));
            our_box.Intersection(check_ray);

            if(distance != null) 
            {
                return new Collider.RaycastHit(ray.start_vector,ray.end_vector,ColHost,(float)distance);
            }
            */
            return null;
        }
        

        public override ModelData? DrawModel()
        {
            return null;
        }
    }
}